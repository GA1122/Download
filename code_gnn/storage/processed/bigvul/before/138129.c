AccessibilityRole AXObject::ariaRoleToWebCoreRole(const String& value) {
  ASSERT(!value.isEmpty());

  static const ARIARoleMap* roleMap = createARIARoleMap();

  Vector<String> roleVector;
  value.split(' ', roleVector);
  AccessibilityRole role = UnknownRole;
  for (const auto& child : roleVector) {
    role = roleMap->at(child);
    if (role)
      return role;
  }

  return role;
}
