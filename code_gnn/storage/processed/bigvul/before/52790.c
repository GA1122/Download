static ssize_t ib_ucm_destroy_id(struct ib_ucm_file *file,
				 const char __user *inbuf,
				 int in_len, int out_len)
{
	struct ib_ucm_destroy_id cmd;
	struct ib_ucm_destroy_id_resp resp;
	struct ib_ucm_context *ctx;
	int result = 0;

	if (out_len < sizeof(resp))
		return -ENOSPC;

	if (copy_from_user(&cmd, inbuf, sizeof(cmd)))
		return -EFAULT;

	mutex_lock(&ctx_id_mutex);
	ctx = idr_find(&ctx_id_table, cmd.id);
	if (!ctx)
		ctx = ERR_PTR(-ENOENT);
	else if (ctx->file != file)
		ctx = ERR_PTR(-EINVAL);
	else
		idr_remove(&ctx_id_table, ctx->id);
	mutex_unlock(&ctx_id_mutex);

	if (IS_ERR(ctx))
		return PTR_ERR(ctx);

	ib_ucm_ctx_put(ctx);
	wait_for_completion(&ctx->comp);

	 
	ib_destroy_cm_id(ctx->cm_id);
	 
	ib_ucm_cleanup_events(ctx);

	resp.events_reported = ctx->events_reported;
	if (copy_to_user((void __user *)(unsigned long)cmd.response,
			 &resp, sizeof(resp)))
		result = -EFAULT;

	kfree(ctx);
	return result;
}