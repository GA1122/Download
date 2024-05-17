void RenderingHelper::RenderTexture(GLuint texture_id) {
  CHECK_EQ(MessageLoop::current(), message_loop_);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  CHECK_EQ(static_cast<int>(glGetError()), GL_NO_ERROR);
  CHECK_EQ(static_cast<int>(eglGetError()), EGL_SUCCESS);
  if (!suppress_swap_to_display_) {
    int window_id = texture_id_to_surface_index_[texture_id];
    CHECK(eglMakeCurrent(egl_display_, egl_surfaces_[window_id],
                         egl_surfaces_[window_id], egl_context_))
        << eglGetError();
    eglSwapBuffers(egl_display_, egl_surfaces_[window_id]);
  }
  CHECK_EQ(static_cast<int>(eglGetError()), EGL_SUCCESS);
}
