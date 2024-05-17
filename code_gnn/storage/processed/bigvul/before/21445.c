static unsigned long elf_map(struct file *filep, unsigned long addr,
		struct elf_phdr *eppnt, int prot, int type,
		unsigned long total_size)
{
	unsigned long map_addr;
	unsigned long size = eppnt->p_filesz + ELF_PAGEOFFSET(eppnt->p_vaddr);
	unsigned long off = eppnt->p_offset - ELF_PAGEOFFSET(eppnt->p_vaddr);
	addr = ELF_PAGESTART(addr);
	size = ELF_PAGEALIGN(size);

	 
	if (!size)
		return addr;

	down_write(&current->mm->mmap_sem);
	 
	if (total_size) {
		total_size = ELF_PAGEALIGN(total_size);
		map_addr = do_mmap(filep, addr, total_size, prot, type, off);
		if (!BAD_ADDR(map_addr))
			do_munmap(current->mm, map_addr+size, total_size-size);
	} else
		map_addr = do_mmap(filep, addr, size, prot, type, off);

	up_write(&current->mm->mmap_sem);
	return(map_addr);
}
