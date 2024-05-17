static scoped_refptr<SiteInstanceImpl> CreateSiteInstance(
    BrowserContext* browser_context,
    const GURL& url) {
  return SiteInstanceImpl::CreateForURL(browser_context, url);
}
