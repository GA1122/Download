static inline int accountable_mapping(struct file *file, vm_flags_t vm_flags)
{
	 
	if (file && is_file_hugepages(file))
		return 0;

	return (vm_flags & (VM_NORESERVE | VM_SHARED | VM_WRITE)) == VM_WRITE;
}
