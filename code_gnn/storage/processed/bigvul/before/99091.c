void ResourceDispatcherHost::NotifyReceivedRedirect(URLRequest* request,
                                                    int child_id,
                                                    const GURL& new_url) {
  FOR_EACH_OBSERVER(Observer, observer_list_,
                    OnReceivedRedirect(this, request, new_url));

  int render_process_id, render_view_id;
  if (!RenderViewForRequest(request, &render_process_id, &render_view_id))
    return;

  CallRenderViewHostResourceDelegate(
      render_process_id, render_view_id,
      &RenderViewHostDelegate::Resource::DidRedirectResource,
      ResourceRedirectDetails(request, GetCertID(request, child_id), new_url));
}
