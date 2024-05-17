void WebGLRenderingContextBase::activeTexture(GLenum texture) {
  if (isContextLost())
    return;
  if (texture - GL_TEXTURE0 >= texture_units_.size()) {
    SynthesizeGLError(GL_INVALID_ENUM, "activeTexture",
                      "texture unit out of range");
    return;
  }
  active_texture_unit_ = texture - GL_TEXTURE0;
  ContextGL()->ActiveTexture(texture);
}
