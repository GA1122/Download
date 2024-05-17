AXObjectInclusion AXObject::accessibilityPlatformIncludesObject() const {
  if (isMenuListPopup() || isMenuListOption())
    return IncludeObject;

  return DefaultBehavior;
}
