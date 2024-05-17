VisiblePosition WebLocalFrameImpl::VisiblePositionForViewportPoint(
    const WebPoint& point_in_viewport) {
  return VisiblePositionForContentsPoint(
      GetFrame()->View()->ViewportToContents(point_in_viewport), GetFrame());
}
