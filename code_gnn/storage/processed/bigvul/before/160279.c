void MimeHandlerViewContainer::OnRenderFrameDestroyed() {
  g_mime_handler_view_container_map.Get().erase(render_frame());
}
