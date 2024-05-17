 bool ResizeGripper::GetAccessibleRole(AccessibilityTypes::Role* role) {
  DCHECK(role);
  *role = AccessibilityTypes::ROLE_SEPARATOR;
  return true;
}
