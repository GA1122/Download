bool SocketStreamDispatcherHost::CanGetCookies(net::SocketStream* socket,
                                               const GURL& url) {
  return content::GetContentClient()->browser()->AllowGetCookie(
      url, url, net::CookieList(), resource_context_, 0, MSG_ROUTING_NONE);
}
