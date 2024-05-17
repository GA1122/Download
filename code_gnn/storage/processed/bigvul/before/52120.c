bool vma_permits_fault(struct vm_area_struct *vma, unsigned int fault_flags)
{
	bool write   = !!(fault_flags & FAULT_FLAG_WRITE);
	bool foreign = !!(fault_flags & FAULT_FLAG_REMOTE);
	vm_flags_t vm_flags = write ? VM_WRITE : VM_READ;

	if (!(vm_flags & vma->vm_flags))
		return false;

	 
	if (!arch_vma_access_permitted(vma, write, false, foreign))
		return false;

	return true;
}
