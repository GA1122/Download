void AXNodeObject::updateAccessibilityRole() {
  bool ignoredStatus = accessibilityIsIgnored();
  m_role = determineAccessibilityRole();

  if (ignoredStatus != accessibilityIsIgnored())
    childrenChanged();
}
