void vm_info_final(struct user_ta_ctx *utc)
{
	if (!utc->vm_info)
		return;

	 
	tlbi_asid(utc->vm_info->asid);

	asid_free(utc->vm_info->asid);
	while (!TAILQ_EMPTY(&utc->vm_info->regions))
		umap_remove_region(utc->vm_info,
				   TAILQ_FIRST(&utc->vm_info->regions));
	free(utc->vm_info);
	utc->vm_info = NULL;
}
