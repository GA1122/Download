void WebGLRenderingContextBase::texImage2D(GLenum target,
                                           GLint level,
                                           GLint internalformat,
                                           GLenum format,
                                           GLenum type,
                                           ImageData* pixels) {
  TexImageHelperImageData(kTexImage2D, target, level, internalformat, 0, format,
                          type, 1, 0, 0, 0, pixels, GetImageDataSize(pixels),
                          0);
}
