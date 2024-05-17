GLboolean WebGL2RenderingContextBase::isSampler(WebGLSampler* sampler) {
  if (isContextLost() || !sampler)
    return 0;

  return ContextGL()->IsSampler(sampler->Object());
}
