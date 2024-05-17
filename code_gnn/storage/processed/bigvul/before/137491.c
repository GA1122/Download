void RunLoop::RemoveNestingObserverOnCurrentThread(NestingObserver* observer) {
  Delegate* delegate = tls_delegate.Get().Get();
  DCHECK(delegate);
  CHECK(delegate->allow_nesting_);
  delegate->nesting_observers_.RemoveObserver(observer);
}
