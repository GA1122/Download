ChromeURLRequestContextGetter::GetURLRequestContext() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));

  if (!url_request_context_.get()) {
    DCHECK(factory_.get());
    url_request_context_ = factory_->Create()->GetWeakPtr();
    factory_.reset();
  }

  CHECK(url_request_context_.get());

  return url_request_context_.get();
}
