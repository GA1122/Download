static int ib_ucm_event_handler(struct ib_cm_id *cm_id,
				struct ib_cm_event *event)
{
	struct ib_ucm_event *uevent;
	struct ib_ucm_context *ctx;
	int result = 0;

	ctx = cm_id->context;

	uevent = kzalloc(sizeof *uevent, GFP_KERNEL);
	if (!uevent)
		goto err1;

	uevent->ctx = ctx;
	uevent->cm_id = cm_id;
	uevent->resp.uid = ctx->uid;
	uevent->resp.id = ctx->id;
	uevent->resp.event = event->event;

	result = ib_ucm_event_process(event, uevent);
	if (result)
		goto err2;

	mutex_lock(&ctx->file->file_mutex);
	list_add_tail(&uevent->file_list, &ctx->file->events);
	list_add_tail(&uevent->ctx_list, &ctx->events);
	wake_up_interruptible(&ctx->file->poll_wait);
	mutex_unlock(&ctx->file->file_mutex);
	return 0;

err2:
	kfree(uevent);
err1:
	 
	return ib_ucm_new_cm_id(event->event);
}