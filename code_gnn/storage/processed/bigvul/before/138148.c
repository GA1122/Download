AXObjectInclusion AXObject::defaultObjectInclusion(
    IgnoredReasons* ignoredReasons) const {
  if (isInertOrAriaHidden()) {
    if (ignoredReasons)
      computeIsInertOrAriaHidden(ignoredReasons);
    return IgnoreObject;
  }

  if (isPresentationalChild()) {
    if (ignoredReasons) {
      AXObject* ancestor = ancestorForWhichThisIsAPresentationalChild();
      ignoredReasons->push_back(
          IgnoredReason(AXAncestorDisallowsChild, ancestor));
    }
    return IgnoreObject;
  }

  return accessibilityPlatformIncludesObject();
}
