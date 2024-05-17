void WebGLRenderingContextBase::texParameteri(GLenum target,
                                              GLenum pname,
                                              GLint param) {
  TexParameter(target, pname, 0, param, false);
}
