"""Grapher

This module scans a set of C-like files and produces a
Graphviz and PDF file showing file dependencies

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
        try:
            return [x[1:-1] for line in open(f'{self.path}{self.name}{self.ext}', 'r') for x in re.findall(r'\s*#\s*include\s+(\S+)', line) if x]
        except:
            # File is not readable
            return []

class GraphSet(object):
    """GraphSet class

    Describe a set and its dependencies
    """
    def __init__(self, dir_list, name='set', mode='file', output='.', known_only=False):
        self.name = name
        self.mode = mode

        if not os.path.isdir(output):
            raise ValueError(f'\'{output}\' is not a directory')

        self.output = output + '/' if not output.endswith('/') else output
        self.known_only = known_only
        self.files = []

        # Get files and their dependencies
        for directory in dir_list:
            if not os.path.isdir(directory):
                raise ValueError(f'\'{directory}\' is not a directory')

            path = directory + '/' if not directory.endswith('/') else directory
            self.files.extend(self.__get_dir_files(path))

    def __str__(self):
        if self.known_only is False:
            return '\n'.join([file.__str__() for file in self.files])
        else:
            return '\n'.join([file.__str__(self.files) for file in self.files])

    def __repr__(self):
        if self.known_only is False:
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

        # Render DOT file
        dot = open(f'{self.output}{self.name}.gv', 'w')

        dot.write(f'digraph "{self.name}" {{\n')

        if self.mode == 'file':
            for file in self.files:
                if self.known_only is False:
                    dot.write(file.__str__() + '\n')
                else:
                    dot.write(file.__str__(self.files) + '\n')

        if self.mode == 'module':
            for file in self.files:
                if self.known_only is False:
                    dot.write(file.__repr__() + '\n')
                else:
                    dot.write(file.__repr__(self.files) + '\n')

        dot.write('}')
        dot.close()

        # Render PDF file (Graphviz must be installed)
        if shutil.which('dot') is None:
            os.remove(f'{self.output}{self.name}.gv')
            raise OSError('Consider installing Graphviz (dot executable not found)')

        subprocess.run(['dot', '-Tpdf', f'{self.output}{self.name}.gv', '-o', f'{self.output}{self.name}.pdf'], check=True)

        if cleanup is True:
            os.remove(f'{self.output}{self.name}.gv')

        if view is True:
            self.__default_open(f'{self.output}{self.name}.pdf')

if __name__ == "__main__":

    # Parse arguments
    parser = argparse.ArgumentParser(description="Render graph dependencies")
    parser.add_argument('-n', '--name', default='set', help="Name of generated file(s) (default is \'set\')")
    parser.add_argument('-m', '--mode', default='file', choices=['file', 'module'], help="Granularity of the graph (Default is \'file\')")
    parser.add_argument('-o', '--output', default='.', help='Directory output (default is execution directory)')
    parser.add_argument('-v', '--verbose', default=False, action="store_true", help="Display graph process in standard output")
    parser.add_argument('--known-only', default=False, action="store_true", help="Render only known files")
    parser.add_argument('--cleanup', default=False, action="store_true", help="Cleanup intermediate files")
    parser.add_argument('--view', default=False, action="store_true", help="Open the rendered result with the default application")
    parser.add_argument('dir', nargs='*', help="Directories containing files to graph")

    args = parser.parse_args()
    if args.verbose is True:
        print(args)

    # Graph set
    graph_set = GraphSet(args.dir, args.name, args.mode, args.output, args.known_only)
    if args.verbose is True and args.mode == 'file':
        print(str(graph_set))
    if args.verbose is True and args.mode == 'module':
        print(repr(graph_set))

    graph_set.render(args.cleanup, args.view)
