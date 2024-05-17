bool ChromeContentRendererClient::HandleSetCookieRequest(
    content::RenderView* sender,
    const GURL& url,
    const GURL& first_party_for_cookies,
    const std::string& value) {
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kChromeFrame)) {
    sender->Send(new ChromeViewHostMsg_SetCookie(
        MSG_ROUTING_NONE, url, first_party_for_cookies, value));
    return true;
  }
  return false;
}
