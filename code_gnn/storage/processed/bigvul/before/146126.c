void WebGL2RenderingContextBase::samplerParameteri(WebGLSampler* sampler,
                                                   GLenum pname,
                                                   GLint param) {
  SamplerParameter(sampler, pname, 0, param, false);
}
