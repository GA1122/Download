void GLES2Implementation::VertexAttribDivisorANGLE(GLuint index,
                                                   GLuint divisor) {
  GPU_CLIENT_SINGLE_THREAD_CHECK();
  GPU_CLIENT_LOG("[" << GetLogPrefix() << "] glVertexAttribDivisorANGLE("
                     << index << ", " << divisor << ") ");
  vertex_array_object_manager_->SetAttribDivisor(index, divisor);
  helper_->VertexAttribDivisorANGLE(index, divisor);
  CheckGLError();
}
