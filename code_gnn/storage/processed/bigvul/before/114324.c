bool WebGraphicsContext3DCommandBufferImpl::isContextLost() {
  return initialize_failed_ ||
      (command_buffer_ && IsCommandBufferContextLost()) ||
      context_lost_reason_ != GL_NO_ERROR;
}
