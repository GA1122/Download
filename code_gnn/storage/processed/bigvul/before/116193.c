void SSLErrorHandler::CompleteContinueRequest() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  DCHECK(!request_has_been_notified_);
  if (request_has_been_notified_)
    return;

  if (delegate_)
    delegate_->ContinueSSLRequest(request_id_);
  request_has_been_notified_ = true;

  Release();
}
