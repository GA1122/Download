ScopedRenderbufferBindingReset::ScopedRenderbufferBindingReset(gl::GLApi* api)
    : api_(api), renderbuffer_(0) {
  api_->glGetIntegervFn(GL_RENDERBUFFER_BINDING, &renderbuffer_);
}
