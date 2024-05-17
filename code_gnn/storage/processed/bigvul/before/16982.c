int WebContext::deleteAllCookies() {
  int request_id = GetNextCookieRequestId();

  context_->GetCookieStore()->DeleteAllAsync(
      base::Bind(&WebContext::DeleteCookiesCallback,
                 weak_factory_.GetWeakPtr(), request_id));

  return request_id;
}
