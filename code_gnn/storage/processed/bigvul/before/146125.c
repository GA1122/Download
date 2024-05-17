void WebGL2RenderingContextBase::samplerParameterf(WebGLSampler* sampler,
                                                   GLenum pname,
                                                   GLfloat param) {
  SamplerParameter(sampler, pname, param, 0, true);
}
