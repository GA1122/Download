void RenderViewHostImpl::OnRouteMessageEvent(
    const ViewMsg_PostMessage_Params& params) {
  delegate_->RouteMessageEvent(this, params);
}
