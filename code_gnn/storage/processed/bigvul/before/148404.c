FindRequestManager* WebContentsImpl::GetOrCreateFindRequestManager() {
  if (FindRequestManager* manager = GetFindRequestManager())
    return manager;

  find_request_manager_.reset(new FindRequestManager(this));

  for (WebContentsImpl* contents : GetWebContentsAndAllInner()) {
    if (contents == this)
      continue;
    if (contents->find_request_manager_) {
      contents->find_request_manager_->StopFinding(
          content::STOP_FIND_ACTION_CLEAR_SELECTION);
      contents->find_request_manager_.release();
    }
  }

  return find_request_manager_.get();
}
