void ResourceMessageFilter::OnLookupSharedWorker(
    const ViewHostMsg_CreateWorker_Params& params, bool* exists, int* route_id,
    bool* url_mismatch) {
  *route_id = render_widget_helper_->GetNextRoutingID();
  *exists = WorkerService::GetInstance()->LookupSharedWorker(
      params.url, params.name, off_the_record(), params.document_id, id(),
      params.render_view_route_id, this, *route_id, url_mismatch);
}
