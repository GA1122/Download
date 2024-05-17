bool RunLoop::IsNestedOnCurrentThread() {
  Delegate* delegate = tls_delegate.Get().Get();
  return delegate && delegate->active_run_loops_.size() > 1;
}
