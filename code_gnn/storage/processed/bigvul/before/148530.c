void WebContentsImpl::RenderViewCreated(RenderViewHost* render_view_host) {
  if (!static_cast<RenderViewHostImpl*>(render_view_host)->is_active())
    return;

  if (delegate_)
    view_->SetOverscrollControllerEnabled(CanOverscrollContent());

  NotificationService::current()->Notify(
      NOTIFICATION_WEB_CONTENTS_RENDER_VIEW_HOST_CREATED,
      Source<WebContents>(this),
      Details<RenderViewHost>(render_view_host));

  view_->RenderViewCreated(render_view_host);

  for (auto& observer : observers_)
    observer.RenderViewCreated(render_view_host);
  RenderFrameDevToolsAgentHost::WebContentsCreated(this);
}
