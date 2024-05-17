void WebContentsImpl::StopFinding(StopFindAction action) {
  Send(new ViewMsg_StopFinding(GetRoutingID(), action));
}
