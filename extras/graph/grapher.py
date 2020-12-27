"""Graph module

This module scans a library and produces PDF files
representing dependencies

You can alter rendered file(s) with command-line options
"""

__version__ = '0.1'
__author__ = 'Marc GIANNETTI'

import subprocess
import argparse
import shutil
import sys
import os
import re

class GraphFile(object):
    """GraphFile class

    Describe a file and its dependencies
    """

    def __init__(self, name, ext, path):
        self.name = name
        self.ext  = ext
        self.path = path
        self.includes = self.__get_include_list()

    def __str__(self, files=None):
        if files is None:
            return f'"{self.name}{self.ext}" -> {{' + ' '.join('"{}"'.format(x) for x in self.includes) + '}'
        else:
            return f'"{self.name}{self.ext}" -> {{' + ' '.join('"{}"'.format(x) for x in self.includes if any((y.name+y.ext) == x for y in files)) + '}'

    def __repr__(self, files=None):
        if files is None:
            return f'"{self.name}" -> {{' + ' '.join('"{}"'.format(os.path.splitext(x)[0]) for x in self.includes if os.path.splitext(x)[0] != self.name) + '}'
        else:
            return f'"{self.name}" -> {{' + ' '.join('"{}"'.format(os.path.splitext(x)[0]) for x in self.includes if os.path.splitext(x)[0] != self.name and any((y.name+y.ext) == x for y in files)) + '}'

    def __get_include_list(self):
        """Get a list of includes from a file"""
        return [x[1:-1] for line in open(f'{self.path}{self.name}{self.ext}', 'r') for x in re.findall(r'^#\s*include\s+(\S+)', line) if x]

class GraphLibrary(object):
    """GraphLibrary class

    Describe a library and its dependencies
    """
    def __init__(self, dir_list, name='grapher', mode='file', existonly=False):
        self.name = name
        self.mode = mode
        self.existonly = existonly
        self.files = []

        # Get files and their dependencies
        for directory in dir_list:
            path = directory + '/' if not directory.endswith('/') else directory
            self.files.extend(self.__get_dir_files(path))

    def __str__(self):
        if self.existonly is False:
            return '\n'.join([file.__str__() for file in self.files])
        else:
            return '\n'.join([file.__str__(self.files) for file in self.files])

    def __repr__(self):
        if self.existonly is False:
            return '\n'.join([file.__repr__() for file in self.files])
        else:
            return '\n'.join([file.__repr__(self.files) for file in self.files])

    def __get_dir_files(self, path):
        """Get a list of files object from a path"""
        return [GraphFile(os.path.splitext(x.name)[0], os.path.splitext(x.name)[1], path) for x in os.scandir(path) if x.is_file()]

    def __default_open(self, path):
        """Open default viewer app according to platform"""
        defaultapp = {
            'linux':'xdg-open',
            'darwin':'open',
            'win32':'start',
            'cygwin':'cygstart'
        }.get(sys.platform)

        if defaultapp is None:
            raise OSError('Sorry, your OS is not handled for auto viewing')

        return subprocess.run([defaultapp, path], check=True)

    def render(self, cleanup=False, view=False):
        """Render graph as a DOT/PDF file"""

        # Write DOT file
        dot = open(f'{self.name}.gv', 'w')

        dot.write(f'digraph {self.name} {{\n')

        if self.mode == 'file':
            for file in self.files:
                if self.existonly is False:
                    dot.write(file.__str__() + '\n')
                else:
                    dot.write(file.__str__(self.files) + '\n')

        if self.mode == 'module':
            for file in self.files:
                if self.existonly is False:
                    dot.write(file.__repr__() + '\n')
                else:
                    dot.write(file.__repr__(self.files) + '\n')

        dot.write('}')
        dot.close()

        # Render PDF file (Graphviz must be installed)
        if shutil.which('dot') is None:
            os.remove(f'{self.name}.gv')
            raise OSError('Consider installing Graphviz (dot executable not found)')

        subprocess.run(['dot', '-Tpdf', f'{self.name}.gv', '-o', f'{self.name}.pdf'], check=True)

        if cleanup is True:
            os.remove(f'{self.name}.gv')

        if view is True:
            self.__default_open(f'{self.name}.pdf')

if __name__ == "__main__":

    # Parse arguments
    parser = argparse.ArgumentParser(description="Render graph library dependencies")
    parser.add_argument('-n', '--name', default='grapher', help='Name of the graph (Default is \'grapher\')')
    parser.add_argument('-m', '--mode', default='file', choices=['file', 'module'], help="Granularity of the graph (Default is \'file\')")
    parser.add_argument('-c', '--cleanup', default=False, action="store_true", help="Cleanup intermediate files")
    parser.add_argument('-o', '--open', default=False, action="store_true", help="Open the rendered result with the default application")
    parser.add_argument('-v', '--verbose', default=False, action="store_true", help="Display graph process in standard output")
    parser.add_argument('--exist-only', default=False, action="store_true", help="Render only accessible files")
    parser.add_argument('dir', nargs='*', help="Directories containing files to graph")

    args = parser.parse_args()
    if args.verbose is True:
        print(args)

    # Graph library
    library = GraphLibrary(args.dir, args.name, args.mode, args.exist_only)
    if args.verbose is True and args.mode == 'file':
        print(str(library))
    if args.verbose is True and args.mode == 'module':
        print(repr(library))

    library.render(args.cleanup, args.open)
