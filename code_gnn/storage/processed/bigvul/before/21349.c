static int policy_vma(struct vm_area_struct *vma, struct mempolicy *new)
{
	int err = 0;
	struct mempolicy *old = vma->vm_policy;

	pr_debug("vma %lx-%lx/%lx vm_ops %p vm_file %p set_policy %p\n",
		 vma->vm_start, vma->vm_end, vma->vm_pgoff,
		 vma->vm_ops, vma->vm_file,
		 vma->vm_ops ? vma->vm_ops->set_policy : NULL);

	if (vma->vm_ops && vma->vm_ops->set_policy)
		err = vma->vm_ops->set_policy(vma, new);
	if (!err) {
		mpol_get(new);
		vma->vm_policy = new;
		mpol_put(old);
	}
	return err;
}
