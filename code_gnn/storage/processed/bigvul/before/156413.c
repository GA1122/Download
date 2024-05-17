void PageHandler::RenderWidgetHostVisibilityChanged(
    RenderWidgetHost* widget_host,
    bool became_visible) {
  if (!screencast_enabled_)
    return;
  NotifyScreencastVisibility(became_visible);
}
