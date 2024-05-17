bool ChromeContentBrowserClient::IsRendererDebugURLBlacklisted(
    const GURL& url,
    content::BrowserContext* context) {
  PolicyBlacklistService* service =
      PolicyBlacklistFactory::GetForBrowserContext(context);

  using URLBlacklistState = policy::URLBlacklist::URLBlacklistState;
  URLBlacklistState blacklist_state = service->GetURLBlacklistState(url);
  return blacklist_state == URLBlacklistState::URL_IN_BLACKLIST;
}
