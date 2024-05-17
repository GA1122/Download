scoped_refptr<SiteInstance> SiteInstanceImpl::GetRelatedSiteInstance(
    const GURL& url) {
  return browsing_instance_->GetSiteInstanceForURL(url);
}
