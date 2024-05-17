bool elf_get_build_id(Backtrace* backtrace, uintptr_t addr, std::string* build_id) {
 uint8_t e_ident[EI_NIDENT];
 if (backtrace->Read(addr, e_ident, SELFMAG) != SELFMAG) {
 return false;
 }

 if (memcmp(e_ident, ELFMAG, SELFMAG) != 0) {
 return false;
 }

 if (backtrace->Read(addr + SELFMAG, e_ident + SELFMAG, EI_NIDENT - SELFMAG) != EI_NIDENT - SELFMAG) {
 return false;
 }

 if (e_ident[EI_CLASS] == ELFCLASS32) {
 return get_build_id<Elf32_Ehdr, Elf32_Phdr, Elf32_Nhdr>(backtrace, addr, e_ident, build_id);
 } else if (e_ident[EI_CLASS] == ELFCLASS64) {
 return get_build_id<Elf64_Ehdr, Elf64_Phdr, Elf64_Nhdr>(backtrace, addr, e_ident, build_id);
 }

 return false;
}
