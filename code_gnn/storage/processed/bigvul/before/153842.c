GLboolean GLES2Implementation::UnmapBufferCHROMIUM(GLuint target) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glUnmapBufferCHROMIUM(" << target
                     << ")");
  GLuint buffer_id;
  if (!GetBoundPixelTransferBuffer(target, "glMapBufferCHROMIUM", &buffer_id)) {
    SetGLError(GL_INVALID_ENUM, "glUnmapBufferCHROMIUM", "invalid target");
  }
  if (!buffer_id) {
    return false;
  }
  BufferTracker::Buffer* buffer = buffer_tracker_->GetBuffer(buffer_id);
  if (!buffer) {
    SetGLError(GL_INVALID_OPERATION, "glUnmapBufferCHROMIUM", "invalid buffer");
    return false;
  }
  if (!buffer->mapped()) {
    SetGLError(GL_INVALID_OPERATION, "glUnmapBufferCHROMIUM", "not mapped");
    return false;
  }
  buffer->set_mapped(false);
  CheckGLError();
  return true;
}
