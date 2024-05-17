bool AXObject::includesARIAWidgetRole(const String& role) {
  static const HashSet<String, CaseFoldingHash>* roleSet =
      createARIARoleWidgetSet();

  Vector<String> roleVector;
  role.split(' ', roleVector);
  for (const auto& child : roleVector) {
    if (roleSet->contains(child))
      return true;
  }
  return false;
}
