# TMLR - Text Markup Language for Terminals or TerMinaL Render

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)

> A universal text markup language designed for easy terminal output formatting, inspired by HTML.

## 📋 Overview

TMLR (Text Markup Language for Terminals) is a lightweight markup language that uses backtick-based syntax for formatting terminal output. Built on C++ and NCurses, it provides a simple way to create styled terminal applications.

## 📝 License

```
 *  Copyright (C) 2024  Joseph Wangai Mwaniki <joewamwaniki@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

For full details, see the `LICENSE` file included with this distribution.
```

## 🚀 How TMLR Works

TMLR is inspired by HTML and uses backticks as delimiters:

```tmlr
``tagname attributes`tag contents ... `
```

### Example

```tmlr
``document style="color=blue;attribute= {bold,underline};"`       
Oh, this is ``    
    text color 
    `love`. // This is a comment

/* This is a multiline comment */
/* This is /* a /* nested /* multiline/*comment */*/*/*/*/
    
     






`
```

## 📜 Rules & Syntax

* **Root Tag**: The document tag is the root tag
* **Bounds**: Nothing should exist outside the bounds of the document tag
* **Delimiters**: Tags begin with two backticks (` `` `). Only whitespace is allowed between them:

```tmlr
`   `tagname attributes`tag contents ... `
```

> Note: This strict whitespace rule helps prevent errors and may be relaxed in future versions.

## 🏗️ Architecture

- **Built with**: C++ and NCurses
- **Target**: Terminal/console applications
- **Output**: Generates styled terminal output

## 🔍 Keywords

terminal, markup-language, text-formatting, ncurses, cpp, console, text-ui, tui, formatting, syntax

## 📄 File Structure

```
.
├── ast.cpp
├── ast.hpp
├── examples
│   ├── documents_tag.tmlr
│   ├── error.tmlr
│   ├── head.tmlr
│   ├── main.tmlr
│   └── one byte file.tmlr
├── lexer.cpp
├── lexer.hpp
├── LICENCE
├── main.cpp
├── makefile
├── parser.cpp
├── parser.hpp
├── potential bugs.md
├── README.md
├── token.cpp
├── token.hpp
├── visitor.cpp
└── visitor.hpp

```

## **LIMITATION**
> * Only parse UTF-8 more than a byte char in the head
---

*Built with C++ and NCurses. For questions or contributions, contact Joseph Wangai Mwaniki at joewamwaniki@gmail.com*
