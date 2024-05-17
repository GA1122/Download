ChromeURLRequestContext::~ChromeURLRequestContext() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  if (load_time_stats_)
    load_time_stats_->UnregisterURLRequestContext(this);
}
