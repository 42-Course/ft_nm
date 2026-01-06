# Currently my program parses all options and files given

For each file it maps it's whole content and then hexdumps.

The next step is to find out:

- [ ] Handle errors if the file contains format errors (ex: file is not an .elf)
- [ ] what's the offset?
- [ ] How to access the Program header?
- [ ] How to access the Section header?
- [ ] How to access the String and Symbol tables?
- [ ] How to access the Relocation entries?
- [ ] How to access the Dynamic tags?
- [ ] How to access the Dynamic tags?
- [ ] How to access the Notes?


https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf
man elf
man 5 elf
man objdump -> man readelf
man readelf