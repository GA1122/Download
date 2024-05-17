void WebContentsImpl::CreateRenderWidgetHostViewForRenderManager(
    RenderViewHost* render_view_host) {
  RenderWidgetHostViewBase* rwh_view = nullptr;
  bool is_guest_in_site_per_process =
      !!browser_plugin_guest_.get() &&
      BrowserPluginGuestMode::UseCrossProcessFramesForGuests();
  if (is_guest_in_site_per_process) {
    RenderWidgetHostViewChildFrame* rwh_view_child =
        new RenderWidgetHostViewChildFrame(render_view_host->GetWidget());
    rwh_view = rwh_view_child;
  } else {
    rwh_view = view_->CreateViewForWidget(render_view_host->GetWidget(), false);
  }

  if (rwh_view)
    rwh_view->SetSize(GetSizeForNewRenderView());
}
