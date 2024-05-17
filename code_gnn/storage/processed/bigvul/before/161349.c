void SetCookiesOnIO(
    net::URLRequestContextGetter* context_getter,
    std::unique_ptr<protocol::Array<Network::CookieParam>> cookies,
    base::OnceCallback<void(bool)> callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);

  for (size_t i = 0; i < cookies->length(); i++) {
    Network::CookieParam* cookie = cookies->get(i);

    base::OnceCallback<void(bool)> once_callback;
    if (i == cookies->length() - 1)
      once_callback = std::move(callback);

    SetCookieOnIO(context_getter, cookie->GetName(), cookie->GetValue(),
                  cookie->GetUrl(""), cookie->GetDomain(""),
                  cookie->GetPath(""), cookie->GetSecure(false),
                  cookie->GetHttpOnly(false), cookie->GetSameSite(""),
                  cookie->GetExpires(-1), std::move(once_callback));
  }
}
