void WebContext::DeliverSetCookiesResponse(
    scoped_refptr<SetCookiesContext> ctxt) {
  client_->CookiesSet(ctxt->request_id, ctxt->failed);
}
