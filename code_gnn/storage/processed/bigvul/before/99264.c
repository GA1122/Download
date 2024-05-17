Blacklist::Match* GetPrivacyBlacklistMatchForURL(
    const GURL& url, ChromeURLRequestContext* context) {
  const Blacklist* blacklist = context->GetPrivacyBlacklist();
  if (!blacklist)
    return NULL;
  return blacklist->FindMatch(url);
}
