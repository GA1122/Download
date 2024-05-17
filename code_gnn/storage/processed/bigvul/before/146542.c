void WebGLRenderingContextBase::stencilOpSeparate(GLenum face,
                                                  GLenum fail,
                                                  GLenum zfail,
                                                  GLenum zpass) {
  if (isContextLost())
    return;
  ContextGL()->StencilOpSeparate(face, fail, zfail, zpass);
}
