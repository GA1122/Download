static inline int check_io_access(struct pt_regs *regs)
{
#if defined(CONFIG_PPC_PMAC) && defined(CONFIG_PPC32)
	unsigned long msr = regs->msr;
	const struct exception_table_entry *entry;
	unsigned int *nip = (unsigned int *)regs->nip;

	if (((msr & 0xffff0000) == 0 || (msr & (0x80000 | 0x40000)))
	    && (entry = search_exception_tables(regs->nip)) != NULL) {
		 
		if (*nip == 0x60000000)		 
			nip -= 2;
		else if (*nip == 0x4c00012c)	 
			--nip;
		if (*nip == 0x7c0004ac || (*nip >> 26) == 3) {
			 
			unsigned int rb;

			--nip;
			rb = (*nip >> 11) & 0x1f;
			printk(KERN_DEBUG "%s bad port %lx at %p\n",
			       (*nip & 0x100)? "OUT to": "IN from",
			       regs->gpr[rb] - _IO_BASE, nip);
			regs->msr |= MSR_RI;
			regs->nip = entry->fixup;
			return 1;
		}
	}
#endif  
	return 0;
}
