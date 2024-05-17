VisiblePosition WebLocalFrameImpl::VisiblePositionForViewportPoint(
    const WebPoint& point_in_viewport) {
  return VisiblePositionForContentsPoint(
      GetFrame()->View()->ViewportToFrame(point_in_viewport), GetFrame());
}
