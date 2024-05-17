void WebContentsImpl::FocusOwningWebContents(
    RenderWidgetHostImpl* render_widget_host) {
  if (!GuestMode::IsCrossProcessFrameGuest(this) && browser_plugin_guest_)
    return;

  RenderWidgetHostImpl* focused_widget =
      GetFocusedRenderWidgetHost(render_widget_host);

  if (focused_widget != render_widget_host &&
      (!focused_widget ||
       focused_widget->delegate() != render_widget_host->delegate())) {
    SetAsFocusedWebContentsIfNecessary();
  }
}
