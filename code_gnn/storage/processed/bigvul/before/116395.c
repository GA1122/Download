bool ChromeContentRendererClient::HandleGetCookieRequest(
    content::RenderView* sender,
    const GURL& url,
    const GURL& first_party_for_cookies,
    std::string* cookies) {
  if (CommandLine::ForCurrentProcess()->HasSwitch(switches::kChromeFrame)) {
    IPC::SyncMessage* msg = new ChromeViewHostMsg_GetCookies(
        MSG_ROUTING_NONE, url, first_party_for_cookies, cookies);
    sender->Send(msg);
    return true;
  }
  return false;
}
