void WebContentsImpl::UpdateTargetURL(RenderViewHost* render_view_host,
                                      const GURL& url) {
  if (fullscreen_widget_routing_id_ != MSG_ROUTING_NONE) {
    RenderWidgetHostView* fs = GetFullscreenRenderWidgetHostView();
    if (fs && fs->GetRenderWidgetHost() != render_view_host->GetWidget())
      return;
  }
  if (delegate_)
    delegate_->UpdateTargetURL(this, url);
}
