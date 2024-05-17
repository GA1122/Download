InputHandler::ScrollStatus LayerTreeHostImpl::RootScrollBegin(
    ScrollState* scroll_state,
    InputHandler::ScrollInputType type) {
  TRACE_EVENT0("cc", "LayerTreeHostImpl::RootScrollBegin");

  ClearCurrentlyScrollingNode();

  return ScrollBeginImpl(scroll_state, OuterViewportScrollNode(), type);
}
