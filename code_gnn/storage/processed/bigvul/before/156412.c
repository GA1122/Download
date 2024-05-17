void PageHandler::RenderWidgetHostDestroyed(RenderWidgetHost* widget_host) {
  observer_.Remove(widget_host);
}
