# GUIDELINE

This guideline describes the indentation to be respected in the `GPR` project.

## Golden rules

- 4-spaces left for each block of instructions (*No tabulations allowed*)
- 2-spaces left for the `goto` label
- 1-space left between control structure and expression
- 1-space before *and* after operators
- No space for unary operators
- 1-blank line at the end of each file to help file printing
- 140 characters max per line for source code (*we are not in the '80s anymore...*)
- No comments on a brace line
- Multiple expressions are surrounded by parenthesis
- Braces are ventilate on the same level as the instruction
- Underscores are mandatory to separate words
- Multithreading and portability when it's possible
- Include directive should be followed by actual functions used in commentary
- Modules should be documented using the `Doxygen` syntax
- `// TODO` comment is used to indicate subsequent changes
- `// WARN` comment is used to question a particular fact

## if-else statements

- Braces are mandatory if at least one statement is using it

```C
if ((expression) && (expression))
{
    // do something
}
else if (expression)
{
    // do something else
}
else
{
    // do something else
}
```

## for statements

- 1-space left between expressions and components

```C
for (i = 0, j = 1; i < 10; i++, j++)
{
    // do something
}
```

## while statements

```C
while (true)
{
    // do something
}
```

## do-while statements

```C
do
{
    // do something
} while (true)
```

## switch statements

- `case` labels are separate with 1-blank line

```C
switch (expression)
{
    case 1:
        // do something
        break;

    case 2:
        // do something
        break;

    case 3:
        // do something
        break;

    default:
        // do something
        break;
}
```

## Pointers

- Pointer qualifiers must be attached to the name and not to the type (*Avoid multiple variable declarations confusion*)

```C
char **var;
char *func(param);
```

## Macros

- Macros are made up with upper-case letters only
- Macros start with the library name (*utils module is an exception*)

```C
#define GPR_DATE_SEC_LEN 19
```

## Enumerations

- Enumerations have a upper-case letter at the beginning of each word
- Enumeration variables are made up with upper-case letters only

```C
enum My_Enum
{
    MY_ENUM_OK,
    MY_ENUM_KO
};
```

## Global variables

- Global variables are made up with upper-case letters only

```C
short My_Global_Variable = 5;
```

## String arrays

- If a string array is null-terminated, show it with the `+ 1` suffix

```C
#define GPR_ARRAY_LEN 9
char array[GPR_ARRAY_LEN + 1] = {0}; // A string of 9 characters long + 1 null-terminated character
```

## Structures

- Structures are made up with lower-case letters only
- Structures are prefixed with the library name
- No `typedef` allowed (*Avoid hidden type confusion*)
- Variable comments are optional but should be aligned if used

```C
struct gpr_my_struct
{
    char *param1;        // Explanation
    unsigned int param2; // Explanation
};
```

## Functions

- Function names start with the library name, followed by the module name (*this rule does not apply for static functions*)
- No spaces between the function name and the parameter list
- 1-space left between each parameter
- Variables should be declared at the beginning of each function (*block-specific variables allowed*)
- Variables should be initialized with a default value at the declaration
- `return` statement can be omitted for `void` functions
- Main instructions set should be ventilated by explanation comments for dense functions
- Function name should be explicit of what the function do (*Verb prefix is a good practice*)

```C
void gpr_time_do_nothing(const char *param1, size_t param2)
{
    size_t ret = 0;
    gpr_time_do_nothing_also(param1, param2, ret);
}
```

## Modules

- Functions like `gpr_xxx_new`, `gpr_xxx_init`, `gpr_xxx_reset` and `gpr_xxx_free` should be declared for each structure if possible
