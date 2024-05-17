GURL SiteInstanceImpl::GetEffectiveURL(BrowserContext* browser_context,
                                       const GURL& url) {
  return GetContentClient()->browser()->GetEffectiveURL(browser_context, url);
}
