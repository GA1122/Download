RunLoop::Delegate::Client* RunLoop::RegisterDelegateForCurrentThread(
    Delegate* delegate) {
  DCHECK(!delegate->bound_);
  DCHECK_CALLED_ON_VALID_THREAD(delegate->bound_thread_checker_);

  DCHECK(!tls_delegate.Get().Get());
  tls_delegate.Get().Set(delegate);
  delegate->bound_ = true;

   return &delegate->client_interface_;
 }
