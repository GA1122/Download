void ResourceDispatcherHost::OnFollowRedirect(
    int request_id,
    bool has_new_first_party_for_cookies,
    const GURL& new_first_party_for_cookies) {
  FollowDeferredRedirect(receiver_->id(), request_id,
                         has_new_first_party_for_cookies,
                         new_first_party_for_cookies);
}