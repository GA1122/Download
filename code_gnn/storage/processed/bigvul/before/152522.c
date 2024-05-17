void RenderFrameImpl::SetHasReceivedUserGestureBeforeNavigation(bool value) {
  Send(new FrameHostMsg_SetHasReceivedUserGestureBeforeNavigation(routing_id_,
                                                                  value));
}
