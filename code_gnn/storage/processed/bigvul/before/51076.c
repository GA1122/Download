static int apparmor_cred_alloc_blank(struct cred *cred, gfp_t gfp)
{
	 
	struct aa_task_cxt *cxt = aa_alloc_task_context(gfp);
	if (!cxt)
		return -ENOMEM;

	cred_cxt(cred) = cxt;
	return 0;
}