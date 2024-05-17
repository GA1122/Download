int __init reserve_elfcorehdr(unsigned long *start, unsigned long *end)
{
	unsigned long length;

	 

	if (elfcorehdr_addr >= ELFCORE_ADDR_MAX)
		return -EINVAL;

	if ((length = vmcore_find_descriptor_size(elfcorehdr_addr)) == 0) {
		elfcorehdr_addr = ELFCORE_ADDR_MAX;
		return -EINVAL;
	}

	*start = (unsigned long)__va(elfcorehdr_addr);
	*end = *start + length;
	return 0;
}
