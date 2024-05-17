void ResourceMessageFilter::OnGetCookiesEnabled(
    const GURL& url,
    const GURL& first_party_for_cookies,
    bool* enabled) {
  *enabled = GetRequestContextForURL(url)->AreCookiesEnabled();
}
