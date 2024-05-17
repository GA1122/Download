void DatabaseMessageFilter::OnChannelClosing() {
  BrowserMessageFilter::OnChannelClosing();
  if (observer_added_) {
    observer_added_ = false;
    BrowserThread::PostTask(
        BrowserThread::FILE, FROM_HERE,
        base::Bind(&DatabaseMessageFilter::RemoveObserver, this));
  }
}
