GURL SiteInstance::GetSiteForURL(BrowserContext* browser_context,
                                 const GURL& url) {
  return SiteInstanceImpl::GetSiteForURL(browser_context, url,
                                         true  );
}
