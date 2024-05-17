float WebLocalFrameImpl::DistanceToNearestFindMatch(
    const WebFloatPoint& point) {
  float nearest_distance;
  EnsureTextFinder().NearestFindMatch(point, &nearest_distance);
  return nearest_distance;
}
