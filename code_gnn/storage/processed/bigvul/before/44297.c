static struct elf_phdr *load_elf_phdrs(struct elfhdr *elf_ex,
				       struct file *elf_file)
{
	struct elf_phdr *elf_phdata = NULL;
	int retval, size, err = -1;

	 
	if (elf_ex->e_phentsize != sizeof(struct elf_phdr))
		goto out;

	 
	if (elf_ex->e_phnum < 1 ||
		elf_ex->e_phnum > 65536U / sizeof(struct elf_phdr))
		goto out;

	 
	size = sizeof(struct elf_phdr) * elf_ex->e_phnum;
	if (size > ELF_MIN_ALIGN)
		goto out;

	elf_phdata = kmalloc(size, GFP_KERNEL);
	if (!elf_phdata)
		goto out;

	 
	retval = kernel_read(elf_file, elf_ex->e_phoff,
			     (char *)elf_phdata, size);
	if (retval != size) {
		err = (retval < 0) ? retval : -EIO;
		goto out;
	}

	 
	err = 0;
out:
	if (err) {
		kfree(elf_phdata);
		elf_phdata = NULL;
	}
	return elf_phdata;
}
