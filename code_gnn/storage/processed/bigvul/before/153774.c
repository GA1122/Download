void GLES2Implementation::MultiDrawElementsInstancedWEBGL(
    GLenum mode,
    const GLsizei* counts,
    GLenum type,
    const GLsizei* offsets,
    const GLsizei* instance_counts,
    GLsizei drawcount) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glMultiDrawElementsInstancedWEBGL("
                     << GLES2Util::GetStringDrawMode(mode) << ", " << counts
                     << ", " << GLES2Util::GetStringIndexType(type) << ", "
                     << offsets << ", " << instance_counts << ", " << drawcount
                     << ")");
  if (drawcount < 0) {
    SetGLError(GL_INVALID_VALUE, "glMultiDrawElementsInstancedWEBGL",
               "drawcount < 0");
    return;
  }
  if (drawcount == 0) {
    return;
  }
  if (vertex_array_object_manager_->bound_element_array_buffer() == 0) {
    SetGLError(GL_INVALID_OPERATION, "glMultiDrawElementsInstancedWEBGL",
               "No element array buffer");
    return;
  }
  if (vertex_array_object_manager_->SupportsClientSideBuffers()) {
    SetGLError(GL_INVALID_OPERATION, "glMultiDrawElementsInstancedWEBGL",
               "Missing array buffer for vertex attribute");
    return;
  }
  MultiDrawElementsInstancedWEBGLHelper(mode, counts, type, offsets,
                                        instance_counts, drawcount);
  CheckGLError();
}
