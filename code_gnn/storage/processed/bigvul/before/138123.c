bool AXObject::accessibilityIsIgnoredByDefault(
    IgnoredReasons* ignoredReasons) const {
  return defaultObjectInclusion(ignoredReasons) == IgnoreObject;
}
