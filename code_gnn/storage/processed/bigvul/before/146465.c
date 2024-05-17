void WebGLRenderingContextBase::compressedTexImage2D(
    GLenum target,
    GLint level,
    GLenum internalformat,
    GLsizei width,
    GLsizei height,
    GLint border,
    MaybeShared<DOMArrayBufferView> data) {
  if (isContextLost())
    return;
  if (!ValidateTexture2DBinding("compressedTexImage2D", target))
    return;
  if (!ValidateCompressedTexFormat("compressedTexImage2D", internalformat))
    return;
  ContextGL()->CompressedTexImage2D(target, level, internalformat, width,
                                    height, border, data.View()->byteLength(),
                                    data.View()->BaseAddressMaybeShared());
}
