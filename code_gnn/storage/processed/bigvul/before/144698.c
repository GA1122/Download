void WebContentsImpl::UpdateMaxPageIDForSiteInstance(
    SiteInstance* site_instance,
    int32_t page_id) {
  if (GetMaxPageIDForSiteInstance(site_instance) < page_id)
    max_page_ids_[site_instance->GetId()] = page_id;
}
