void ResourceDispatcherHostImpl::OnFollowRedirect(
    int request_id,
    bool has_new_first_party_for_cookies,
    const GURL& new_first_party_for_cookies) {
  ResourceLoader* loader = GetLoader(filter_->child_id(), request_id);
  if (!loader) {
    DVLOG(1) << "OnFollowRedirect for invalid request";
    return;
  }

  ResourceRequestInfoImpl* info = loader->GetRequestInfo();
  if (info->async_handler()) {
    info->async_handler()->OnFollowRedirect(
        has_new_first_party_for_cookies,
        new_first_party_for_cookies);
  }
}
