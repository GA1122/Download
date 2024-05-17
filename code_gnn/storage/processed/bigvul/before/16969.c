void WebContext::SetCookieCallback(scoped_refptr<SetCookiesContext> ctxt,
                                   const QNetworkCookie& cookie,
                                   bool success) {
  DCHECK_GT(ctxt->remaining, 0);

  if (!success) {
    ctxt->failed.push_back(cookie);
  }

  if (--ctxt->remaining > 0) {
    return;
  }

  DeliverSetCookiesResponse(ctxt);
}
