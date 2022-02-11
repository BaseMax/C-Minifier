# C Minifier

A **C program** to remove comments and **minimize** the code (remove whitespace and summarize code)

## Features

- Supporting most **C keywords** and automatically add space after these, such as: `if, else, int, long, float, bool, sizeof`
- Supporting **macro** statements #include and others
- Fast and Without any third-party library
- Without using **Regex**
- Remove inline C-style comments
- Remove multiline C-style comments
- Trim whitespaces
- Remove empty lines

## Build

```
$ gcc Minifier.c -o minifier
```

## Using

If you are going to minify A C program and get the results in stdout:

```
./minifier Minifier.c
```

And maybe sometime you need to write the result in another or same file:

```
./minifier Minifier.c o.c
```

### TODO

- Adding more C keywords if needed or missed
- Automatically rename and make short identifier names (A hash-map list to keep original name and new random name and a function to generate a short (and unique) name)

Welcome to help and feel free to contribute and send Pull request.

Please open a issue if you find a bug and not have time to fix. Thanks in advance.

© Copyright Max Base, 2022
