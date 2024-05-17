void RenderViewHostImpl::SetHasPendingCrossSiteRequest(bool has_pending_request,
                                                       int request_id) {
  CrossSiteRequestManager::GetInstance()->SetHasPendingCrossSiteRequest(
      GetProcess()->GetID(), GetRoutingID(), has_pending_request);
  pending_request_id_ = request_id;
}
