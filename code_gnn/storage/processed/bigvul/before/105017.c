HttpBridge::RequestContext::~RequestContext() {
  DCHECK(BrowserThread::CurrentlyOn(BrowserThread::IO));
  delete http_transaction_factory();
}
