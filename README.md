# TMLR

```ascii
Copyright (C) 2025 Joseph Wangai Mwaniki

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

For full details, see the `LICENSE` file included with this distribution.
```

> This is a universal text mark up language made as an attempt to make easy working with terminal output

## how tmlr works

> tmlr works is inspired by html. it uses backticks as delimiters.

```tmlr
``tagname attributes`tag contents ... `
```

***sweet example***
```tmlr
``document style="color=blue;attribute= {bold,underline};"`       
Oh, this is ``    
    text color 
    `love`. // This is a comment

/* This is a multiline comment */
/* This is /* a /* nested /* multiline/*comment */*/*/*/*/
    
     






`
```

### rules

* The root tag is the document tag.
* Nothing should exist outside the bounds of the document tag.
* The begining of the tag is signaled by `` two backticks. And there can ony exist whitespace between the two head delimiter. eg.

    ```tmlr
    `   `tagname attributes`tag contents ... `
    ```

    > this is to reduce the chances of err. maybe in a future day that this shall be lifted.

