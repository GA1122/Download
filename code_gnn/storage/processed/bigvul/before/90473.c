static void rdma_umap_close(struct vm_area_struct *vma)
{
	struct ib_uverbs_file *ufile = vma->vm_file->private_data;
	struct rdma_umap_priv *priv = vma->vm_private_data;

	if (!priv)
		return;

	 
	mutex_lock(&ufile->umap_lock);
	list_del(&priv->list);
	mutex_unlock(&ufile->umap_lock);
	kfree(priv);
}
