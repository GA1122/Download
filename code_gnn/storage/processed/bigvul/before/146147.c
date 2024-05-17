void WebGL2RenderingContextBase::texStorage3D(GLenum target,
                                              GLsizei levels,
                                              GLenum internalformat,
                                              GLsizei width,
                                              GLsizei height,
                                              GLsizei depth) {
  if (isContextLost() ||
      !ValidateTexStorage("texStorage3D", target, levels, internalformat, width,
                          height, depth, kTexStorageType3D))
    return;

  ContextGL()->TexStorage3D(target, levels, internalformat, width, height,
                            depth);
}
