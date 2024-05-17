static void shm_open(struct vm_area_struct *vma)
{
	int err = __shm_open(vma);
	 
	WARN_ON_ONCE(err);
}
