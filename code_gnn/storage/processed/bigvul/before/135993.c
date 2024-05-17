  bool CanAccessDataForOrigin(const GURL& gurl) {
    if (origin_lock_.is_empty())
      return true;
    GURL site_gurl = SiteInstanceImpl::GetSiteForURL(NULL, gurl);
    return origin_lock_ == site_gurl;
  }
