bool SocketStreamDispatcherHost::CanSetCookie(net::SocketStream* request,
                                              const GURL& url,
                                              const std::string& cookie_line,
                                              net::CookieOptions* options) {
  return content::GetContentClient()->browser()->AllowSetCookie(
      url, url, cookie_line, resource_context_, 0, MSG_ROUTING_NONE, options);
}
