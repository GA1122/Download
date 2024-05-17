bool SiteInstance::IsSameWebSite(BrowserContext* browser_context,
                                 const GURL& real_src_url,
                                 const GURL& real_dest_url) {
  return SiteInstanceImpl::IsSameWebSite(browser_context, real_src_url,
                                         real_dest_url, true);
}
