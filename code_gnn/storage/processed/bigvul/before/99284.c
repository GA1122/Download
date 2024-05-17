void ResourceMessageFilter::OnDeleteCookie(const GURL& url,
                                           const std::string& cookie_name) {
  URLRequestContext* context = GetRequestContextForURL(url);
  context->cookie_store()->DeleteCookie(url, cookie_name);
}
