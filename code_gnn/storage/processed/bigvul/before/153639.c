void GLES2Implementation::EnableVertexAttribArray(GLuint index) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glEnableVertexAttribArray("
                     << index << ")");
  vertex_array_object_manager_->SetAttribEnable(index, true);
  helper_->EnableVertexAttribArray(index);
  CheckGLError();
}
