void WebGLRenderingContextBase::generateMipmap(GLenum target) {
  if (isContextLost())
    return;
  if (!ValidateTextureBinding("generateMipmap", target))
    return;
  ContextGL()->GenerateMipmap(target);
}
