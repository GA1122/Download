bool BrowserContextIOData::CanAccessCookies(const GURL& url,
                                            const GURL& first_party_url,
                                            bool write) {
  net::StaticCookiePolicy policy(GetCookiePolicy());
  if (write) {
    return policy.CanSetCookie(url, first_party_url) == net::OK;
  }

  return policy.CanGetCookies(url, first_party_url) == net::OK;
}
