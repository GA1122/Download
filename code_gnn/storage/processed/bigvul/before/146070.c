void WebGL2RenderingContextBase::compressedTexImage3D(
    GLenum target,
    GLint level,
    GLenum internalformat,
    GLsizei width,
    GLsizei height,
    GLsizei depth,
    GLint border,
    MaybeShared<DOMArrayBufferView> data,
    GLuint src_offset,
    GLuint src_length_override) {
  if (isContextLost())
    return;
  if (bound_pixel_unpack_buffer_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "compressedTexImage3D",
                      "a buffer is bound to PIXEL_UNPACK_BUFFER");
    return;
  }
  if (!ValidateTexture3DBinding("compressedTexImage3D", target))
    return;
  if (!ValidateCompressedTexFormat("compressedTexImage3D", internalformat))
    return;
  if (src_offset > data.View()->byteLength()) {
    SynthesizeGLError(GL_INVALID_VALUE, "compressedTexImage3D",
                      "srcOffset is out of range");
    return;
  }
  if (src_length_override == 0) {
    src_length_override = data.View()->byteLength() - src_offset;
  } else if (src_length_override > data.View()->byteLength() - src_offset) {
    SynthesizeGLError(GL_INVALID_VALUE, "compressedTexImage3D",
                      "srcLengthOverride is out of range");
    return;
  }
  ContextGL()->CompressedTexImage3D(
      target, level, internalformat, width, height, depth, border,
      src_length_override,
      static_cast<uint8_t*>(data.View()->BaseAddressMaybeShared()) +
          src_offset);
}
