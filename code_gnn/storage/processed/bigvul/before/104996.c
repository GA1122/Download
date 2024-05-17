void HttpBridge::Abort() {
  base::AutoLock lock(fetch_state_lock_);
  DCHECK(!fetch_state_.aborted);
  if (fetch_state_.aborted || fetch_state_.request_completed)
    return;

  fetch_state_.aborted = true;
  BrowserThread::DeleteSoon(BrowserThread::IO, FROM_HERE,
                            fetch_state_.url_poster);
  fetch_state_.url_poster = NULL;
  fetch_state_.os_error_code = net::ERR_ABORTED;
  http_post_completed_.Signal();
}
