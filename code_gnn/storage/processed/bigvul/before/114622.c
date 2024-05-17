RenderThreadImpl* RenderThreadImpl::current() {
  return lazy_tls.Pointer()->Get();
}
