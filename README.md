# Lexical Analyzer (C)

A lexical analyzer (tokenizer) written in C for a custom programming language, developed for the Programming Languages course at Ege University.

**Group project — developed with Büşra Karakütük.**

## What It Does
The program reads a source file written in a custom language and breaks it down into tokens (keywords, identifiers, operators, constants, etc.), writing the result to an output file.

- Input: `<filename>.tj` (source code)
- Output: `<filename>.lx` (token stream)

## Recognized Tokens
- **Keywords:** new, int, text, size, subs, locate, insert, override, read, write, from, to, input, output, asText, asString
- **Operators:** `+`, `-`, `:=`
- **Delimiters:** `(`, `)`, `;`
- **Constants:** integer constants, string constants (`"..."`)
- **Identifiers:** up to 30 characters
- **Comments:** `/* ... */` blocks are skipped

## Error Handling
The analyzer reports lexical errors for:
- Invalid characters
- Malformed `:=` operator
- Identifiers longer than 30 characters
- Unterminated string constants or comments

## Tech Stack
- C (standard library: `stdio.h`, `stdlib.h`, `string.h`, `ctype.h`)

## How to Run
```bash
gcc plproje.c -o plproje
./plproje test1
```
This reads `test1.tj` and produces `test1.lx`.

## Test Files
The repository includes 9 sample input/output pairs (`test1.tj`–`test9.tj` and their corresponding `.lx` outputs) demonstrating the analyzer on different inputs.
