FindRequestManager* WebContentsImpl::GetFindRequestManager() {
  for (WebContentsImpl* contents = this; contents;
       contents = contents->GetOuterWebContents()) {
    if (contents->find_request_manager_)
      return contents->find_request_manager_.get();
  }

  return nullptr;
}
