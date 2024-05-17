void WebGL2RenderingContextBase::texStorage2D(GLenum target,
                                              GLsizei levels,
                                              GLenum internalformat,
                                              GLsizei width,
                                              GLsizei height) {
  if (isContextLost() ||
      !ValidateTexStorage("texStorage2D", target, levels, internalformat, width,
                          height, 1, kTexStorageType2D))
    return;

  ContextGL()->TexStorage2DEXT(target, levels, internalformat, width, height);
}
