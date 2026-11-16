# Text editor

A terminal line editor in C with file I/O and snapshot-based undo/redo. Text is stored in linked lists; editing creates a new state on the history stack.

## Build and run

Requires Clang or GCC and GNU Make. Run from this directory:

```sh
make -j4
./build/debug/editor notes.txt
```

Use `help` to list commands. `append`, `insert`, `replace`, `delete`, and `clear` change the buffer; `undo` and `redo` navigate history. Line numbers start at 1. `save` writes the file; `quit` exits without saving automatically.

Use `make test` for library checks, `make release` for an optimized build, and `make sanitize` for AddressSanitizer/UBSan. Binaries are stored in `build/<mode>/`; `make clean` removes them. Prompts and file loading use 1,024-byte line buffers; longer lines are split or truncated. History stores a full copy per edit.
