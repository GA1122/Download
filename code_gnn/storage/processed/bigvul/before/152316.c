void RenderFrameImpl::ForwardResourceTimingToParent(
    const blink::WebResourceTimingInfo& info) {
  Send(new FrameHostMsg_ForwardResourceTimingToParent(
      routing_id_, WebResourceTimingInfoToResourceTimingInfo(info)));
}
