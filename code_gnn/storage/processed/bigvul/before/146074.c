void WebGL2RenderingContextBase::compressedTexSubImage3D(
    GLenum target,
    GLint level,
    GLint xoffset,
    GLint yoffset,
    GLint zoffset,
    GLsizei width,
    GLsizei height,
    GLsizei depth,
    GLenum format,
    MaybeShared<DOMArrayBufferView> data,
    GLuint src_offset,
    GLuint src_length_override) {
  if (isContextLost())
    return;
  if (bound_pixel_unpack_buffer_) {
    SynthesizeGLError(GL_INVALID_OPERATION, "compressedTexSubImage3D",
                      "a buffer is bound to PIXEL_UNPACK_BUFFER");
    return;
  }
  if (!ValidateTexture3DBinding("compressedTexSubImage3D", target))
    return;
  if (!ValidateCompressedTexFormat("compressedTexSubImage3D", format))
    return;
  if (src_offset > data.View()->byteLength()) {
    SynthesizeGLError(GL_INVALID_VALUE, "compressedTexSubImage3D",
                      "srcOffset is out of range");
    return;
  }
  if (src_length_override == 0) {
    src_length_override = data.View()->byteLength() - src_offset;
  } else if (src_length_override > data.View()->byteLength() - src_offset) {
    SynthesizeGLError(GL_INVALID_VALUE, "compressedTexSubImage3D",
                      "srcLengthOverride is out of range");
    return;
  }
  ContextGL()->CompressedTexSubImage3D(
      target, level, xoffset, yoffset, zoffset, width, height, depth, format,
      src_length_override,
      static_cast<uint8_t*>(data.View()->BaseAddressMaybeShared()) +
          src_offset);
}
