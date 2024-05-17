void ResourceDispatcherHostImpl::FollowDeferredRedirect(
    int child_id,
    int request_id,
    bool has_new_first_party_for_cookies,
    const GURL& new_first_party_for_cookies) {
  PendingRequestList::iterator i = pending_requests_.find(
      GlobalRequestID(child_id, request_id));
  if (i == pending_requests_.end() || !i->second->status().is_success()) {
    DVLOG(1) << "FollowDeferredRedirect for invalid request";
    return;
  }

  if (has_new_first_party_for_cookies)
    i->second->set_first_party_for_cookies(new_first_party_for_cookies);
  i->second->FollowDeferredRedirect();
}
