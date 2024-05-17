bool AXListBoxOption::computeAccessibilityIsIgnored(
    IgnoredReasons* ignoredReasons) const {
  if (!getNode())
    return true;

  if (accessibilityIsIgnoredByDefault(ignoredReasons))
    return true;

  return false;
}
