static void ib_ucm_cleanup_events(struct ib_ucm_context *ctx)
{
	struct ib_ucm_event *uevent;

	mutex_lock(&ctx->file->file_mutex);
	list_del(&ctx->file_list);
	while (!list_empty(&ctx->events)) {

		uevent = list_entry(ctx->events.next,
				    struct ib_ucm_event, ctx_list);
		list_del(&uevent->file_list);
		list_del(&uevent->ctx_list);
		mutex_unlock(&ctx->file->file_mutex);

		 
		if (ib_ucm_new_cm_id(uevent->resp.event))
			ib_destroy_cm_id(uevent->cm_id);

		kfree(uevent);
		mutex_lock(&ctx->file->file_mutex);
	}
	mutex_unlock(&ctx->file->file_mutex);
}