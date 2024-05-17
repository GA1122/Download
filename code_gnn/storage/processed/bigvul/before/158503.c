size_t WebLocalFrameImpl::CharacterIndexForPoint(
    const WebPoint& point_in_viewport) const {
  if (!GetFrame())
    return kNotFound;

  HitTestLocation location(
      GetFrame()->View()->ViewportToFrame(point_in_viewport));
  HitTestResult result = GetFrame()->GetEventHandler().HitTestResultAtLocation(
      location, HitTestRequest::kReadOnly | HitTestRequest::kActive);
  return GetFrame()->Selection().CharacterIndexForPoint(
      result.RoundedPointInInnerNodeFrame());
}
