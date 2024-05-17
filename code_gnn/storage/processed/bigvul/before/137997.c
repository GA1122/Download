AXObject* AXLayoutObject::treeAncestorDisallowingChild() const {
  AXObject* axObj = parentObject();
  AXObject* treeAncestor = 0;
  while (axObj) {
    if (axObj->isTree()) {
      treeAncestor = axObj;
      break;
    }
    axObj = axObj->parentObject();
  }

  if (treeAncestor) {
    AccessibilityRole role = roleValue();
    if (role != TreeItemRole && role != StaticTextRole)
      return treeAncestor;
  }
  return 0;
}
