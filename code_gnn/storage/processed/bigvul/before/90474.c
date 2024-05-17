static void rdma_umap_open(struct vm_area_struct *vma)
{
	struct ib_uverbs_file *ufile = vma->vm_file->private_data;
	struct rdma_umap_priv *opriv = vma->vm_private_data;
	struct rdma_umap_priv *priv;

	if (!opriv)
		return;

	 
	if (!down_read_trylock(&ufile->hw_destroy_rwsem))
		goto out_zap;
	 
	if (!ufile->ucontext)
		goto out_unlock;

	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
	if (!priv)
		goto out_unlock;
	rdma_umap_priv_init(priv, vma);

	up_read(&ufile->hw_destroy_rwsem);
	return;

out_unlock:
	up_read(&ufile->hw_destroy_rwsem);
out_zap:
	 
	vma->vm_private_data = NULL;
	zap_vma_ptes(vma, vma->vm_start, vma->vm_end - vma->vm_start);
}
