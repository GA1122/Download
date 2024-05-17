void SetCookieOnIO(net::URLRequestContextGetter* context_getter,
                   const std::string& name,
                   const std::string& value,
                   const std::string& url_spec,
                   const std::string& domain,
                   const std::string& path,
                   bool secure,
                   bool http_only,
                   const std::string& same_site,
                   double expires,
                   base::OnceCallback<void(bool)> callback) {
  DCHECK_CURRENTLY_ON(BrowserThread::IO);
  net::URLRequestContext* request_context =
      context_getter->GetURLRequestContext();

  if (url_spec.empty() && domain.empty()) {
    std::move(callback).Run(false);
    return;
  }

  std::string normalized_domain = domain;
  if (!url_spec.empty()) {
    GURL source_url = GURL(url_spec);
    if (!source_url.SchemeIsHTTPOrHTTPS()) {
      std::move(callback).Run(false);
      return;
    }

    secure = secure || source_url.SchemeIsCryptographic();
    if (normalized_domain.empty())
      normalized_domain = source_url.host();
  }

  GURL url = GURL((secure ? "https://" : "http://") + normalized_domain);
  if (!normalized_domain.empty() && normalized_domain[0] != '.')
    normalized_domain = "";

  base::Time expiration_date;
  if (expires >= 0) {
    expiration_date =
        expires ? base::Time::FromDoubleT(expires) : base::Time::UnixEpoch();
  }

  net::CookieSameSite css = net::CookieSameSite::NO_RESTRICTION;
  if (same_site == Network::CookieSameSiteEnum::Lax)
    css = net::CookieSameSite::LAX_MODE;
  if (same_site == Network::CookieSameSiteEnum::Strict)
    css = net::CookieSameSite::STRICT_MODE;

  std::unique_ptr<net::CanonicalCookie> cc(
      net::CanonicalCookie::CreateSanitizedCookie(
          url, name, value, normalized_domain, path, base::Time(),
          expiration_date, base::Time(), secure, http_only, css,
          net::COOKIE_PRIORITY_DEFAULT));
  if (!cc) {
    std::move(callback).Run(false);
    return;
  }
  request_context->cookie_store()->SetCanonicalCookieAsync(
      std::move(cc), secure, true  , std::move(callback));
}
