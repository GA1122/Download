bool DownloadRequestLimiter::TabDownloadState::NotifyCallbacks(bool allow) {
  std::vector<DownloadRequestLimiter::Callback> callbacks;
  bool throttled = false;

  if (!allow || (callbacks_.size() < kMaxDownloadsAtOnce)) {
    factory_.InvalidateWeakPtrs();
    callbacks.swap(callbacks_);
  } else {
    std::vector<DownloadRequestLimiter::Callback>::iterator start, end;
    start = callbacks_.begin();
    end = callbacks_.begin() + kMaxDownloadsAtOnce;
    callbacks.assign(start, end);
    callbacks_.erase(start, end);
    throttled = true;
  }

  for (const auto& callback : callbacks) {
    BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
                            base::BindOnce(callback, allow));
  }

  return throttled;
}
