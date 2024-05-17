void WebGLRenderingContextBase::stencilOp(GLenum fail,
                                          GLenum zfail,
                                          GLenum zpass) {
  if (isContextLost())
    return;
  ContextGL()->StencilOp(fail, zfail, zpass);
}
