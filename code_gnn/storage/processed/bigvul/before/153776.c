void GLES2Implementation::MultiDrawElementsWEBGL(GLenum mode,
                                                 const GLsizei* counts,
                                                 GLenum type,
                                                 const GLsizei* offsets,
                                                 GLsizei drawcount) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glMultiDrawElementsWEBGL("
                     << GLES2Util::GetStringDrawMode(mode) << ", " << counts
                     << ", " << GLES2Util::GetStringIndexType(type) << ", "
                     << offsets << ", " << drawcount << ")");
  if (drawcount < 0) {
    SetGLError(GL_INVALID_VALUE, "glMultiDrawElementsWEBGL", "drawcount < 0");
    return;
  }
  if (drawcount == 0) {
    return;
  }
  if (vertex_array_object_manager_->bound_element_array_buffer() == 0) {
    SetGLError(GL_INVALID_OPERATION, "glMultiDrawElementsWEBGL",
               "No element array buffer");
    return;
  }
  if (vertex_array_object_manager_->SupportsClientSideBuffers()) {
    SetGLError(GL_INVALID_OPERATION, "glMultiDrawElementsWEBGL",
               "Missing array buffer for vertex attribute");
    return;
  }
  MultiDrawElementsWEBGLHelper(mode, counts, type, offsets, drawcount);
  CheckGLError();
}
