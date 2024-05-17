scoped_refptr<SiteInstance> SiteInstance::CreateForURL(
    BrowserContext* browser_context,
    const GURL& url) {
  return SiteInstanceImpl::CreateForURL(browser_context, url);
}
