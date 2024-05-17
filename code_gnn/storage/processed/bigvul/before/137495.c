RunLoop::Delegate::~Delegate() {
  DCHECK_CALLED_ON_VALID_THREAD(bound_thread_checker_);
  if (bound_)
    tls_delegate.Get().Set(nullptr);
}
