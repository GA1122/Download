void WebLocalFrameImpl::SelectRange(const WebPoint& base_in_viewport,
                                    const WebPoint& extent_in_viewport) {
  MoveRangeSelection(base_in_viewport, extent_in_viewport);
}
