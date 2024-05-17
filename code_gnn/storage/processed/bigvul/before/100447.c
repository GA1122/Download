RenderThread* RenderThread::current() {
  return lazy_tls.Pointer()->Get();
}
