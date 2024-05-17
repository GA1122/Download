static inline bool arch_syscall_match_sym_name(const char *sym, const char *name)
{
	 
	return !strcmp(sym + 3, name + 3);
}
