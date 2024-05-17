void RunLoop::DisallowNestingOnCurrentThread() {
  tls_delegate.Get().Get()->allow_nesting_ = false;
}
