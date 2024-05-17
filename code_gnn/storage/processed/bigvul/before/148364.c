void WebContentsImpl::DragSourceEndedAt(int client_x,
                                        int client_y,
                                        int screen_x,
                                        int screen_y,
                                        blink::WebDragOperation operation,
                                        RenderWidgetHost* source_rwh) {
  if (browser_plugin_embedder_.get())
    browser_plugin_embedder_->DragSourceEndedAt(
        client_x, client_y, screen_x, screen_y, operation);
  if (source_rwh) {
    source_rwh->DragSourceEndedAt(gfx::Point(client_x, client_y),
                                  gfx::Point(screen_x, screen_y),
                                  operation);
  }
}
