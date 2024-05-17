void RenderFrameHostImpl::OnBubbleLogicalScrollInParentFrame(
    blink::WebScrollDirection direction,
    blink::WebScrollGranularity granularity) {
  if (!is_active())
    return;

  RenderFrameProxyHost* proxy =
      frame_tree_node()->render_manager()->GetProxyToParent();
  if (!proxy) {
    bad_message::ReceivedBadMessage(GetProcess(),
                                    bad_message::RFH_NO_PROXY_TO_PARENT);
    return;
  }

  proxy->BubbleLogicalScroll(direction, granularity);
}
