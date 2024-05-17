bool Resource::WillFollowRedirect(const ResourceRequest& new_request,
                                  const ResourceResponse& redirect_response) {
  if (is_revalidating_)
    RevalidationFailed();
  redirect_chain_.push_back(RedirectPair(new_request, redirect_response));
  return true;
}
