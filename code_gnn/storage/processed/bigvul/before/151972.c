std::set<int> RenderFrameHostImpl::GetNavigationEntryIdsPendingCommit() {
  std::set<int> result;
  if (navigation_request_)
    result.insert(navigation_request_->nav_entry_id());
  for (auto const& requests : navigation_requests_)
    result.insert(requests.second->nav_entry_id());
  return result;
}
