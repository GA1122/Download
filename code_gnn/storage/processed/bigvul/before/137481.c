bool RunLoop::IsNestingAllowedOnCurrentThread() {
  return tls_delegate.Get().Get()->allow_nesting_;
}
