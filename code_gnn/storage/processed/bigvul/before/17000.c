int WebContext::setCookies(const QUrl& url,
                           const QList<QNetworkCookie>& cookies) {
  int request_id = GetNextCookieRequestId();

  net::CookieStore* cookie_store = context_->GetCookieStore();
  scoped_refptr<SetCookiesContext> ctxt = new SetCookiesContext(request_id);
  
  for (int i = 0; i < cookies.size(); ++i) {
    const QNetworkCookie& cookie = cookies.at(i);

    if (cookie.name().isEmpty()) {
      ctxt->failed.push_back(cookie);
      continue;
    }

    base::Time expiry;
    if (cookie.expirationDate().isValid()) {
      expiry = base::Time::FromJsTime(cookie.expirationDate().toMSecsSinceEpoch());
    }

    ctxt->remaining++;

    cookie_store->SetCookieWithDetailsAsync(
        GURL(url.toString().toStdString()),
        std::string(cookie.name().constData()),
        std::string(cookie.value().constData()),
        std::string(cookie.domain().toUtf8().constData()),
        std::string(cookie.path().toUtf8().constData()),
        base::Time(),
        expiry,
        base::Time(),
        cookie.isSecure(),
        cookie.isHttpOnly(),
        net::CookieSameSite::DEFAULT_MODE,
        false,  
        net::COOKIE_PRIORITY_DEFAULT,
        base::Bind(&WebContext::SetCookieCallback,
                   weak_factory_.GetWeakPtr(), ctxt, cookie));
  }

  if (ctxt->remaining == 0) {
    base::MessageLoop::current()->PostTask(
        FROM_HERE,
        base::Bind(&WebContext::DeliverSetCookiesResponse,
                   weak_factory_.GetWeakPtr(), ctxt));
  }

  return request_id;
}
