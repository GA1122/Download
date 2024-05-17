bool HttpBridge::MakeSynchronousPost(int* os_error_code, int* response_code) {
  DCHECK_EQ(MessageLoop::current(), created_on_loop_);
  if (DCHECK_IS_ON()) {
    base::AutoLock lock(fetch_state_lock_);
    DCHECK(!fetch_state_.request_completed);
  }
  DCHECK(url_for_request_.is_valid()) << "Invalid URL for request";
  DCHECK(!content_type_.empty()) << "Payload not set";

  if (!BrowserThread::PostTask(
          BrowserThread::IO, FROM_HERE,
          NewRunnableMethod(this, &HttpBridge::CallMakeAsynchronousPost))) {
    LOG(WARNING) << "Could not post CallMakeAsynchronousPost task";
    return false;
  }

  if (!http_post_completed_.Wait())   
    NOTREACHED();                     

  base::AutoLock lock(fetch_state_lock_);
  DCHECK(fetch_state_.request_completed || fetch_state_.aborted);
  *os_error_code = fetch_state_.os_error_code;
  *response_code = fetch_state_.http_response_code;
  return fetch_state_.request_succeeded;
}
