bool WebGraphicsContext3DCommandBufferImpl::isContextLost() {
  return initialize_failed_ ||
      (context_ && context_->IsCommandBufferContextLost()) ||
      context_lost_reason_ != GL_NO_ERROR;
}
