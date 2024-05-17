WGC3Denum WebGraphicsContext3DCommandBufferImpl::getGraphicsResetStatusARB() {
  if (context_->IsCommandBufferContextLost() &&
      context_lost_reason_ == GL_NO_ERROR) {
    return GL_UNKNOWN_CONTEXT_RESET_ARB;
  }

  return context_lost_reason_;
}
