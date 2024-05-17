 bool AXTableCell::computeAccessibilityIsIgnored(
    IgnoredReasons* ignoredReasons) const {
  AXObjectInclusion decision = defaultObjectInclusion(ignoredReasons);
  if (decision == IncludeObject)
    return false;
  if (decision == IgnoreObject)
    return true;

  if (!isTableCell())
    return AXLayoutObject::computeAccessibilityIsIgnored(ignoredReasons);

  return false;
}
