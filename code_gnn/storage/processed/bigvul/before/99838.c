void WebPluginProxy::SetCookie(const GURL& url,
                               const GURL& first_party_for_cookies,
                               const std::string& cookie) {
  Send(new PluginHostMsg_SetCookie(route_id_, url,
                                   first_party_for_cookies, cookie));
}
