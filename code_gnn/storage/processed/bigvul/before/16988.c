int WebContext::getCookies(const QUrl& url) {
  int request_id = GetNextCookieRequestId();

  context_->GetCookieStore()->GetAllCookiesForURLAsync(
      GURL(url.toString().toStdString()),
      base::Bind(&WebContext::GetCookiesCallback,
                 weak_factory_.GetWeakPtr(), request_id));

  return request_id;
}
