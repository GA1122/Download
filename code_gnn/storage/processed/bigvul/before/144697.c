void WebContentsImpl::UpdateMaxPageID(int32_t page_id) {
  UpdateMaxPageIDForSiteInstance(GetSiteInstance(), page_id);
}
