ScopedTexture2DBindingReset::ScopedTexture2DBindingReset(gl::GLApi* api)
    : api_(api), texture_(0) {
  api_->glGetIntegervFn(GL_TEXTURE_2D_BINDING_EXT, &texture_);
}
