# ft_nm

A reimplementation of the GNU `nm` utility that displays symbol information from ELF binary files.

## What It Does

ft_nm parses ELF (Executable and Linkable Format) binary files and displays their symbol table in a format compatible with the GNU nm tool. It supports both 32-bit and 64-bit ELF binaries and provides various filtering and sorting options.

For each file, the program:
1. Memory-maps the entire file content using `mmap(2)`
2. Parses the ELF header to determine file format (32-bit vs 64-bit)
3. Locates the symbol table (`.symtab`) section
4. Extracts all symbols with their addresses, types, and names
5. Filters symbols based on command-line options
6. Sorts symbols alphabetically (unless disabled)
7. Prints the results in nm-compatible format

## Implementation Details

### ftlist - The Linked List Abstraction

The implementation leverages a custom linked list library (`t_list` from libft) that provides significant advantages:

- **Separation of Concerns**: Operations like extraction, filtering, and sorting are cleanly separated using iterator functions (`ft_lstiter`, `ft_lstiter_param`)
- **Reusable Operations**: Generic list operations like `ft_lstsort`, `ft_lstfilter`, `ft_lstadd_back` allow for modular, testable code
- **Custom Comparators**: The sorting algorithm accepts function pointers, enabling flexible comparison logic (alphanumeric, case-insensitive, reverse)
- **Memory Management**: Centralized cleanup with `ft_lstclear` and custom destructors prevents memory leaks
- **Functional Style**: Operations like `ft_lstmap` and `ft_lstfilter_param` enable a functional programming approach

This design allows the main `nm()` function to be a simple pipeline:
```c
extract_nm_symbols() → filter_symbols() → sort_symbols() → print_nm_symbols()
```

### Section Name Resolution

When a symbol's `st_name` field is 0 (typically for `STT_SECTION` type symbols), the implementation retrieves the section name from the **section header string table** (`.shstrtab`) rather than the regular string table (`.strtab`).

This is done by:
1. Identifying the symbol as `STT_SECTION` type
2. Using the symbol's `st_shndx` to index into the section header array
3. Reading the section's `sh_name` field
4. Looking up that offset in `.shstrtab` instead of `.strtab`

This ensures that section symbols like `.text`, `.data`, `.bss` are properly named.

### Address Formatting: 32-bit vs 64-bit

The symbol address printing differs based on the binary architecture:

- **64-bit binaries**: Addresses are printed as 16 hexadecimal characters (e.g., `00000000004005a0`)
  - Format: `%016lx`

- **32-bit binaries**: Addresses are printed as 8 hexadecimal characters (e.g., `08048400`)
  - Format: `%08lx`

For **undefined symbols** (`SHN_UNDEF`), no address is printed—instead, the appropriate number of spaces is displayed (17 spaces for 64-bit, 9 spaces for 32-bit) to maintain column alignment.

## Supported Options

- `-a, --debug-syms`: Display debugger-only symbols
- `-g, --extern-only`: Display only external symbols
- `-u, --undefined-only`: Display only undefined symbols
- `-r, --reverse-sort`: Reverse the sorting order
- `-p, --no-sort`: Do not sort symbols (display in original order)
- `-h, --help`: Display usage information

## Symbol Sorting

By default, symbols are sorted alphabetically by name using an alphanumeric comparison that:
1. Ignores non-alphanumeric characters during comparison
2. Performs case-insensitive matching
3. Falls back to strcmp for identical alphanumeric sequences

The sorting behavior for symbols with identical names is undefined (matches GNU nm behavior).

## References

- [System V ABI x86-64 Specification](https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf)
- `man elf`, `man 5 elf`
- `man nm`, `man readelf`, `man objdump`