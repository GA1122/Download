SYSCALL_DEFINE4(osf_set_program_attributes, unsigned long, text_start,
		unsigned long, text_len, unsigned long, bss_start,
		unsigned long, bss_len)
{
	struct mm_struct *mm;

	mm = current->mm;
	mm->end_code = bss_start + bss_len;
	mm->start_brk = bss_start + bss_len;
	mm->brk = bss_start + bss_len;
#if 0
	printk("set_program_attributes(%lx %lx %lx %lx)\n",
		text_start, text_len, bss_start, bss_len);
#endif
	return 0;
}
