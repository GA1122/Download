void ResourceDispatcherHost::NotifyResponseStarted(URLRequest* request,
                                                   int child_id) {
  FOR_EACH_OBSERVER(Observer, observer_list_, OnRequestStarted(this, request));

  int render_process_id, render_view_id;
  if (!RenderViewForRequest(request, &render_process_id, &render_view_id))
    return;

  CallRenderViewHostResourceDelegate(
      render_process_id, render_view_id,
      &RenderViewHostDelegate::Resource::DidStartReceivingResourceResponse,
      ResourceRequestDetails(request, GetCertID(request, child_id)));
}
