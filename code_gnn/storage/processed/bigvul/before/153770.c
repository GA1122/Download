void GLES2Implementation::MultiDrawArraysInstancedWEBGL(
    GLenum mode,
    const GLint* firsts,
    const GLsizei* counts,
    const GLsizei* instance_counts,
    GLsizei drawcount) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glMultiDrawArraysInstancedWEBGL("
                     << GLES2Util::GetStringDrawMode(mode) << ", " << firsts
                     << ", " << counts << ", " << instance_counts << ", "
                     << drawcount << ")");
  if (drawcount < 0) {
    SetGLError(GL_INVALID_VALUE, "glMultiDrawArraysWEBGLInstanced",
               "drawcount < 0");
    return;
  }
  if (drawcount == 0) {
    return;
  }
  if (vertex_array_object_manager_->SupportsClientSideBuffers()) {
    SetGLError(GL_INVALID_OPERATION, "glMultiDrawArraysWEBGLInstanced",
               "Missing array buffer for vertex attribute");
    return;
  }
  MultiDrawArraysInstancedWEBGLHelper(mode, firsts, counts, instance_counts,
                                      drawcount);
  CheckGLError();
}
