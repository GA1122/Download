void SSLErrorHandler::CompleteCancelRequest(int error) {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  DCHECK(!request_has_been_notified_);
  if (request_has_been_notified_)
    return;

  SSLCertErrorHandler* cert_error = AsSSLCertErrorHandler();
  const SSLInfo* ssl_info = NULL;
  if (cert_error)
    ssl_info = &cert_error->ssl_info();
  if (delegate_)
    delegate_->CancelSSLRequest(request_id_, error, ssl_info);
  request_has_been_notified_ = true;

  Release();
}
