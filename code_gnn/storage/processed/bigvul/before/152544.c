void RenderFrameImpl::SuddenTerminationDisablerChanged(
    bool present,
    blink::WebSuddenTerminationDisablerType disabler_type) {
  Send(new FrameHostMsg_SuddenTerminationDisablerChanged(routing_id_, present,
                                                         disabler_type));
}
