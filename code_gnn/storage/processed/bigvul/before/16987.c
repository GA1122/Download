int WebContext::getAllCookies() {
  int request_id = GetNextCookieRequestId();

  context_->GetCookieStore()->GetAllCookiesAsync(
      base::Bind(&WebContext::GetCookiesCallback,
                 weak_factory_.GetWeakPtr(), request_id));

  return request_id;
}
