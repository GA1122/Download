void GLES2Implementation::ResizeCHROMIUM(GLuint width,
                                         GLuint height,
                                         float scale_factor,
                                         GLenum color_space,
                                         GLboolean alpha) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glResizeCHROMIUM(" << width << ", "
                     << height << ", " << scale_factor << ", " << alpha << ")");
  helper_->ResizeCHROMIUM(width, height, scale_factor, color_space, alpha);
  CheckGLError();
}
