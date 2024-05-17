void WebGLRenderingContextBase::texParameterf(GLenum target,
                                              GLenum pname,
                                              GLfloat param) {
  TexParameter(target, pname, param, 0, true);
}
