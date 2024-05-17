void ContextState::UpdateWindowRectanglesForBoundDrawFramebufferClientID(
    GLuint client_id) {
  bool old_id_nonzero = current_draw_framebuffer_client_id != 0;
  bool new_id_nonzero = client_id != 0;
  current_draw_framebuffer_client_id = client_id;
  if (old_id_nonzero ^ new_id_nonzero) {
    UpdateWindowRectangles();
  }
}
