void WebGL2RenderingContextBase::compressedTexSubImage2D(
    GLenum target,
    GLint level,
    GLint xoffset,
    GLint yoffset,
    GLsizei width,
    GLsizei height,
    GLenum format,
    MaybeShared<DOMArrayBufferView> data,
    GLuint src_offset,
    GLuint src_length_override) {
  if (isContextLost())
    return;
  if (bound_pixel_unpack_buffer_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "compressedTexSubImage2D",
                      "a buffer is bound to PIXEL_UNPACK_BUFFER");
    return;
  }
  if (!ValidateTexture2DBinding("compressedTexSubImage2D", target))
    return;
  if (!ValidateCompressedTexFormat("compressedTexSubImage2D", format))
    return;
  if (src_offset > data.View()->byteLength()) {
    SynthesizeGLError(GL_INVALID_VALUE, "compressedTexSubImage2D",
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
  ContextGL()->CompressedTexSubImage2D(
      target, level, xoffset, yoffset, width, height, format,
      src_length_override,
      static_cast<uint8_t*>(data.View()->BaseAddressMaybeShared()) +
          src_offset);
}
