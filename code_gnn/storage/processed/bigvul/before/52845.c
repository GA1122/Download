static int ucma_free_ctx(struct ucma_context *ctx)
{
	int events_reported;
	struct ucma_event *uevent, *tmp;
	LIST_HEAD(list);


	ucma_cleanup_multicast(ctx);

	 
	mutex_lock(&ctx->file->mut);
	list_for_each_entry_safe(uevent, tmp, &ctx->file->event_list, list) {
		if (uevent->ctx == ctx)
			list_move_tail(&uevent->list, &list);
	}
	list_del(&ctx->list);
	mutex_unlock(&ctx->file->mut);

	list_for_each_entry_safe(uevent, tmp, &list, list) {
		list_del(&uevent->list);
		if (uevent->resp.event == RDMA_CM_EVENT_CONNECT_REQUEST)
			rdma_destroy_id(uevent->cm_id);
		kfree(uevent);
	}

	events_reported = ctx->events_reported;
	kfree(ctx);
	return events_reported;
}