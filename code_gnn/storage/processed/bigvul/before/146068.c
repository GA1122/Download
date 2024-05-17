void WebGL2RenderingContextBase::compressedTexImage2D(
    GLenum target,
    GLint level,
    GLenum internalformat,
    GLsizei width,
    GLsizei height,
    GLint border,
    MaybeShared<DOMArrayBufferView> data,
    GLuint src_offset,
    GLuint src_length_override) {
  if (isContextLost())
    return;
  if (bound_pixel_unpack_buffer_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "compressedTexImage2D",
                      "a buffer is bound to PIXEL_UNPACK_BUFFER");
    return;
  }
  if (!ValidateTexture2DBinding("compressedTexImage2D", target))
    return;
  if (!ValidateCompressedTexFormat("compressedTexImage2D", internalformat))
    return;
  if (src_offset > data.View()->byteLength()) {
    SynthesizeGLError(GL_INVALID_VALUE, "compressedTexImage2D",
                      "srcOffset is out of range");
    return;
  }
  if (src_length_override == 0) {
    src_length_override = data.View()->byteLength() - src_offset;
  } else if (src_length_override > data.View()->byteLength() - src_offset) {
    SynthesizeGLError(GL_INVALID_VALUE, "compressedTexImage2D",
                      "srcLengthOverride is out of range");
    return;
  }
  ContextGL()->CompressedTexImage2D(
      target, level, internalformat, width, height, border, src_length_override,
      static_cast<uint8_t*>(data.View()->BaseAddressMaybeShared()) +
          src_offset);
}
