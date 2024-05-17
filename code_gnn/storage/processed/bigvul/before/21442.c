static int elf_core_dump(struct coredump_params *cprm)
{
	int has_dumped = 0;
	mm_segment_t fs;
	int segs;
	size_t size = 0;
	struct vm_area_struct *vma, *gate_vma;
	struct elfhdr *elf = NULL;
	loff_t offset = 0, dataoff, foffset;
	struct elf_note_info info;
	struct elf_phdr *phdr4note = NULL;
	struct elf_shdr *shdr4extnum = NULL;
	Elf_Half e_phnum;
	elf_addr_t e_shoff;

	 
  
	 
	elf = kmalloc(sizeof(*elf), GFP_KERNEL);
	if (!elf)
		goto out;
	 
	segs = current->mm->map_count;
	segs += elf_core_extra_phdrs();

	gate_vma = get_gate_vma(current->mm);
	if (gate_vma != NULL)
		segs++;

	 
	segs++;

	 
	e_phnum = segs > PN_XNUM ? PN_XNUM : segs;

	 
	if (!fill_note_info(elf, e_phnum, &info, cprm->signr, cprm->regs))
		goto cleanup;

	has_dumped = 1;
	current->flags |= PF_DUMPCORE;
  
	fs = get_fs();
	set_fs(KERNEL_DS);

	offset += sizeof(*elf);				 
	offset += segs * sizeof(struct elf_phdr);	 
	foffset = offset;

	 
	{
		size_t sz = get_note_info_size(&info);

		sz += elf_coredump_extra_notes_size();

		phdr4note = kmalloc(sizeof(*phdr4note), GFP_KERNEL);
		if (!phdr4note)
			goto end_coredump;

		fill_elf_note_phdr(phdr4note, sz, offset);
		offset += sz;
	}

	dataoff = offset = roundup(offset, ELF_EXEC_PAGESIZE);

	offset += elf_core_vma_data_size(gate_vma, cprm->mm_flags);
	offset += elf_core_extra_data_size();
	e_shoff = offset;

	if (e_phnum == PN_XNUM) {
		shdr4extnum = kmalloc(sizeof(*shdr4extnum), GFP_KERNEL);
		if (!shdr4extnum)
			goto end_coredump;
		fill_extnum_info(elf, shdr4extnum, e_shoff, segs);
	}

	offset = dataoff;

	size += sizeof(*elf);
	if (size > cprm->limit || !dump_write(cprm->file, elf, sizeof(*elf)))
		goto end_coredump;

	size += sizeof(*phdr4note);
	if (size > cprm->limit
	    || !dump_write(cprm->file, phdr4note, sizeof(*phdr4note)))
		goto end_coredump;

	 
	for (vma = first_vma(current, gate_vma); vma != NULL;
			vma = next_vma(vma, gate_vma)) {
		struct elf_phdr phdr;

		phdr.p_type = PT_LOAD;
		phdr.p_offset = offset;
		phdr.p_vaddr = vma->vm_start;
		phdr.p_paddr = 0;
		phdr.p_filesz = vma_dump_size(vma, cprm->mm_flags);
		phdr.p_memsz = vma->vm_end - vma->vm_start;
		offset += phdr.p_filesz;
		phdr.p_flags = vma->vm_flags & VM_READ ? PF_R : 0;
		if (vma->vm_flags & VM_WRITE)
			phdr.p_flags |= PF_W;
		if (vma->vm_flags & VM_EXEC)
			phdr.p_flags |= PF_X;
		phdr.p_align = ELF_EXEC_PAGESIZE;

		size += sizeof(phdr);
		if (size > cprm->limit
		    || !dump_write(cprm->file, &phdr, sizeof(phdr)))
			goto end_coredump;
	}

	if (!elf_core_write_extra_phdrs(cprm->file, offset, &size, cprm->limit))
		goto end_coredump;

 	 
	if (!write_note_info(&info, cprm->file, &foffset))
		goto end_coredump;

	if (elf_coredump_extra_notes_write(cprm->file, &foffset))
		goto end_coredump;

	 
	if (!dump_seek(cprm->file, dataoff - foffset))
		goto end_coredump;

	for (vma = first_vma(current, gate_vma); vma != NULL;
			vma = next_vma(vma, gate_vma)) {
		unsigned long addr;
		unsigned long end;

		end = vma->vm_start + vma_dump_size(vma, cprm->mm_flags);

		for (addr = vma->vm_start; addr < end; addr += PAGE_SIZE) {
			struct page *page;
			int stop;

			page = get_dump_page(addr);
			if (page) {
				void *kaddr = kmap(page);
				stop = ((size += PAGE_SIZE) > cprm->limit) ||
					!dump_write(cprm->file, kaddr,
						    PAGE_SIZE);
				kunmap(page);
				page_cache_release(page);
			} else
				stop = !dump_seek(cprm->file, PAGE_SIZE);
			if (stop)
				goto end_coredump;
		}
	}

	if (!elf_core_write_extra_data(cprm->file, &size, cprm->limit))
		goto end_coredump;

	if (e_phnum == PN_XNUM) {
		size += sizeof(*shdr4extnum);
		if (size > cprm->limit
		    || !dump_write(cprm->file, shdr4extnum,
				   sizeof(*shdr4extnum)))
			goto end_coredump;
	}

end_coredump:
	set_fs(fs);

cleanup:
	free_note_info(&info);
	kfree(shdr4extnum);
	kfree(phdr4note);
	kfree(elf);
out:
	return has_dumped;
}