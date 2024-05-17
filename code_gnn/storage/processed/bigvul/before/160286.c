MimeHandlerViewContainer::~MimeHandlerViewContainer() {
  if (loader_) {
    DCHECK(is_embedded_);
    loader_->Cancel();
  }

  if (render_frame()) {
    g_mime_handler_view_container_map.Get()[render_frame()].erase(this);
    if (g_mime_handler_view_container_map.Get()[render_frame()].empty())
      g_mime_handler_view_container_map.Get().erase(render_frame());
  }
}
