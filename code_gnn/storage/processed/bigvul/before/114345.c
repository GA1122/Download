void WebGraphicsContext3DCommandBufferImpl::synthesizeGLError(
    WGC3Denum error) {
  if (std::find(synthetic_errors_.begin(), synthetic_errors_.end(), error) ==
      synthetic_errors_.end()) {
    synthetic_errors_.push_back(error);
  }
}
