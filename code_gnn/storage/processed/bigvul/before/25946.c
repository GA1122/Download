out_of_memory(struct pt_regs *regs, unsigned long error_code,
	      unsigned long address)
{
	 
	up_read(&current->mm->mmap_sem);

	pagefault_out_of_memory();
}
