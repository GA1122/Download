static void ucma_removal_event_handler(struct rdma_cm_id *cm_id)
{
	struct ucma_context *ctx = cm_id->context;
	struct ucma_event *con_req_eve;
	int event_found = 0;

	if (ctx->destroying)
		return;

	 
	if (ctx->cm_id == cm_id) {
		mutex_lock(&mut);
		ctx->closing = 1;
		mutex_unlock(&mut);
		queue_work(ctx->file->close_wq, &ctx->close_work);
		return;
	}

	list_for_each_entry(con_req_eve, &ctx->file->event_list, list) {
		if (con_req_eve->cm_id == cm_id &&
		    con_req_eve->resp.event == RDMA_CM_EVENT_CONNECT_REQUEST) {
			list_del(&con_req_eve->list);
			INIT_WORK(&con_req_eve->close_work, ucma_close_event_id);
			queue_work(ctx->file->close_wq, &con_req_eve->close_work);
			event_found = 1;
			break;
		}
	}
	if (!event_found)
		pr_err("ucma_removal_event_handler: warning: connect request event wasn't found\n");
}