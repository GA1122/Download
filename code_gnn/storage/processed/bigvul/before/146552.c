void WebGLRenderingContextBase::texSubImage2D(GLenum target,
                                              GLint level,
                                              GLint xoffset,
                                              GLint yoffset,
                                              GLenum format,
                                              GLenum type,
                                              ImageData* pixels) {
  TexImageHelperImageData(kTexSubImage2D, target, level, 0, 0, format, type, 1,
                          xoffset, yoffset, 0, pixels, GetImageDataSize(pixels),
                          0);
}
