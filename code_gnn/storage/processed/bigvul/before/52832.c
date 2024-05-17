static int ucma_close(struct inode *inode, struct file *filp)
{
	struct ucma_file *file = filp->private_data;
	struct ucma_context *ctx, *tmp;

	mutex_lock(&file->mut);
	list_for_each_entry_safe(ctx, tmp, &file->ctx_list, list) {
		ctx->destroying = 1;
		mutex_unlock(&file->mut);

		mutex_lock(&mut);
		idr_remove(&ctx_idr, ctx->id);
		mutex_unlock(&mut);

		flush_workqueue(file->close_wq);
		 
		mutex_lock(&mut);
		if (!ctx->closing) {
			mutex_unlock(&mut);
			 
			rdma_destroy_id(ctx->cm_id);
		} else {
			mutex_unlock(&mut);
		}

		ucma_free_ctx(ctx);
		mutex_lock(&file->mut);
	}
	mutex_unlock(&file->mut);
	destroy_workqueue(file->close_wq);
	kfree(file);
	return 0;
}
