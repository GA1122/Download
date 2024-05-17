const AtomicString& AXObject::roleName(AccessibilityRole role) {
  static const Vector<AtomicString>* roleNameVector = createRoleNameVector();

  return roleNameVector->at(role);
}
