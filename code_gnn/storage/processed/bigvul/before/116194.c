void SSLErrorHandler::CompleteTakeNoAction() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  DCHECK(!request_has_been_notified_);
  if (request_has_been_notified_)
    return;

  request_has_been_notified_ = true;

  Release();
}
