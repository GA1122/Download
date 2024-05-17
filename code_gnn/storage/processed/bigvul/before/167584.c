scoped_refptr<SiteInstanceImpl> SiteInstanceImpl::Create(
    BrowserContext* browser_context) {
  return base::WrapRefCounted(
      new SiteInstanceImpl(new BrowsingInstance(browser_context)));
}
