media::Context3D GetSharedMainThreadContext3D() {
  cc::ContextProvider* provider =
      RenderThreadImpl::current()->SharedMainThreadContextProvider().get();
  if (!provider)
    return media::Context3D();
  return media::Context3D(provider->ContextGL(), provider->GrContext());
}
