bool ImageView::GetAccessibleRole(AccessibilityTypes::Role* role) {
  if (!role)
    return false;

  *role = AccessibilityTypes::ROLE_GRAPHIC;
  return true;
}
