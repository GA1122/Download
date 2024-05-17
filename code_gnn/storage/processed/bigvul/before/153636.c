void GLES2Implementation::DrawRangeElements(GLenum mode,
                                            GLuint start,
                                            GLuint end,
                                            GLsizei count,
                                            GLenum type,
                                            const void* indices) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glDrawRangeElements("
                     << GLES2Util::GetStringDrawMode(mode) << ", " << start
                     << ", " << end << ", " << count << ", "
                     << GLES2Util::GetStringIndexType(type) << ", "
                     << static_cast<const void*>(indices) << ")");
  if (end < start) {
    SetGLError(GL_INVALID_VALUE, "glDrawRangeElements", "end < start");
    return;
  }
  DrawElementsImpl(mode, count, type, indices, "glDrawRangeElements");
}
