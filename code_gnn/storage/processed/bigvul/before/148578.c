void WebContentsImpl::StopFinding(StopFindAction action) {
  if (FindRequestManager* manager = GetFindRequestManager())
    manager->StopFinding(action);
}
