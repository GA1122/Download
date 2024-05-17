ChildThread* ChildThread::current() {
  return g_lazy_tls.Pointer()->Get();
}
