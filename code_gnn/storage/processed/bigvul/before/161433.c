  void AddObserverOnIOThread() {
    DCHECK_CURRENTLY_ON(BrowserThread::IO);
    context_->AddObserver(this);
  }
