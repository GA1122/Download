  void SignalDone() {
    DCHECK_CURRENTLY_ON(BrowserThread::IO);
    if (!is_removing_)
      return;
    is_removing_ = false;
    event_->Signal();
  }
