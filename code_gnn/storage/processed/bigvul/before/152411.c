void RenderFrameImpl::NavigateBackForwardSoon(int offset,
                                              bool has_user_gesture) {
  render_view()->NavigateBackForwardSoon(offset, has_user_gesture);
  Send(new FrameHostMsg_GoToEntryAtOffset(GetRoutingID(), offset,
                                          has_user_gesture));
}
