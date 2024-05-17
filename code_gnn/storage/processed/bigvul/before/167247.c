void FetchManager::Loader::Abort() {
  if (resolver_) {
    resolver_->Reject(DOMException::Create(kAbortError));
    resolver_.Clear();
  }
  if (threadable_loader_) {
    auto loader = threadable_loader_;
    threadable_loader_ = nullptr;
    loader->Cancel();
  }
  probe::didFailFetch(execution_context_, this);
  NotifyFinished();
}
