SiteInstanceImpl::~SiteInstanceImpl() {
  GetContentClient()->browser()->SiteInstanceDeleting(this);

  if (process_)
    process_->RemoveObserver(this);

  if (has_site_)
    browsing_instance_->UnregisterSiteInstance(this);
}
