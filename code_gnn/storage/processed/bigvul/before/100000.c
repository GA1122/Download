void WebPluginImpl::SetCookie(const GURL& url,
                              const GURL& first_party_for_cookies,
                              const std::string& cookie) {
  WebKit::webKitClient()->setCookies(
      url, first_party_for_cookies, WebString::fromUTF8(cookie));
}
