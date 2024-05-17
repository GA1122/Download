void ResourceDispatcherHost::OnSetCookieBlocked(URLRequest* request) {
  RESOURCE_LOG("OnSetCookieBlocked: " << request->url().spec());

  int render_process_id, render_view_id;
  if (!RenderViewForRequest(request, &render_process_id, &render_view_id))
    return;

  CallRenderViewHostResourceDelegate(
      render_process_id, render_view_id,
      &RenderViewHostDelegate::Resource::OnContentBlocked,
      CONTENT_SETTINGS_TYPE_COOKIES);
}
