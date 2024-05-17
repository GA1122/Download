void DeleteSelectedCookiesOnIO(net::URLRequestContextGetter* context_getter,
                               const std::string& name,
                               const std::string& url_spec,
                               const std::string& domain,
                               const std::string& path,
                               base::OnceClosure callback,
                               const net::CookieList& cookie_list) {
  net::URLRequestContext* request_context =
      context_getter->GetURLRequestContext();
  std::string normalized_domain = domain;
  if (normalized_domain.empty()) {
    GURL url(url_spec);
    if (!url.SchemeIsHTTPOrHTTPS()) {
      std::move(callback).Run();
      return;
    }
    normalized_domain = url.host();
  }

  net::CookieList filtered_list;
  for (const auto& cookie : cookie_list) {
    if (cookie.Name() != name)
      continue;
    if (cookie.Domain() != normalized_domain)
      continue;
    if (!path.empty() && cookie.Path() != path)
      continue;
    filtered_list.push_back(cookie);
  }

  for (size_t i = 0; i < filtered_list.size(); ++i) {
    const auto& cookie = filtered_list[i];
    base::OnceCallback<void(uint32_t)> once_callback;
    if (i == filtered_list.size() - 1)
      once_callback = base::BindOnce(&DeletedCookiesOnIO, std::move(callback));
    request_context->cookie_store()->DeleteCanonicalCookieAsync(
        cookie, std::move(once_callback));
  }
  if (!filtered_list.size())
    std::move(callback).Run();
}
