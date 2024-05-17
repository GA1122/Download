const AtomicString& AXObject::internalRoleName(AccessibilityRole role) {
  static const Vector<AtomicString>* internalRoleNameVector =
      createInternalRoleNameVector();

  return internalRoleNameVector->at(role);
}
