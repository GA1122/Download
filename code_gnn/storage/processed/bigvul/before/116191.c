void SSLErrorHandler::CancelRequest() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::UI));

  BrowserThread::PostTask(
      BrowserThread::IO, FROM_HERE,
      base::Bind(
          &SSLErrorHandler::CompleteCancelRequest, this, net::ERR_ABORTED));
}
