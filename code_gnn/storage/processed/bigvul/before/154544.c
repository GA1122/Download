ScopedRenderbufferBindingReset::~ScopedRenderbufferBindingReset() {
  api_->glBindRenderbufferEXTFn(GL_RENDERBUFFER, renderbuffer_);
}
