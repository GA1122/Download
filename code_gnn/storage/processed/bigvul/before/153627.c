void GLES2Implementation::DestroyImageCHROMIUM(GLuint image_id) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glDestroyImageCHROMIUM("
                     << image_id << ")");
  DestroyImageCHROMIUMHelper(image_id);
  CheckGLError();
}
