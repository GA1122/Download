void ResourceMessageFilter::OnCreateWorker(
    const ViewHostMsg_CreateWorker_Params& params, int* route_id) {
  *route_id = params.route_id != MSG_ROUTING_NONE ?
      params.route_id : render_widget_helper_->GetNextRoutingID();
  WorkerService::GetInstance()->CreateWorker(
      params.url, params.is_shared, off_the_record(), params.name,
      params.document_id, id(), params.render_view_route_id, this, *route_id,
      db_dispatcher_host_->database_tracker(),
      GetRequestContextForURL(params.url)->host_content_settings_map());
}
