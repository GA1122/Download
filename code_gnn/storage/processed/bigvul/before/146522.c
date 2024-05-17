bool WebGLRenderingContextBase::isContextLost() const {
  return context_lost_mode_ != kNotLostContext;
}
