void WebGLRenderingContextBase::compressedTexSubImage2D(
    GLenum target,
    GLint level,
    GLint xoffset,
    GLint yoffset,
    GLsizei width,
    GLsizei height,
    GLenum format,
    MaybeShared<DOMArrayBufferView> data) {
  if (isContextLost())
    return;
  if (!ValidateTexture2DBinding("compressedTexSubImage2D", target))
    return;
  if (!ValidateCompressedTexFormat("compressedTexSubImage2D", format))
    return;
  ContextGL()->CompressedTexSubImage2D(
      target, level, xoffset, yoffset, width, height, format,
      data.View()->byteLength(), data.View()->BaseAddressMaybeShared());
}
