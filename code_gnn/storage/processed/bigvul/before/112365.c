bool ResourceDispatcherHostImpl::RenderViewForRequest(
    const net::URLRequest* request,
    int* render_process_id,
    int* render_view_id) {
  const ResourceRequestInfoImpl* info =
      ResourceRequestInfoImpl::ForRequest(request);
  if (!info) {
    *render_process_id = -1;
    *render_view_id = -1;
    return false;
  }

  return info->GetAssociatedRenderView(render_process_id, render_view_id);
}
