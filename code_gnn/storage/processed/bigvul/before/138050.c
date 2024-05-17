unsigned AXNodeObject::hierarchicalLevel() const {
  Node* node = this->getNode();
  if (!node || !node->isElementNode())
    return 0;

  Element* element = toElement(node);
  String levelStr = element->getAttribute(aria_levelAttr);
  if (!levelStr.isEmpty()) {
    int level = levelStr.toInt();
    if (level > 0)
      return level;
    return 1;
  }

  if (roleValue() != TreeItemRole)
    return 0;

  unsigned level = 1;
  for (AXObject* parent = parentObject(); parent;
       parent = parent->parentObject()) {
    AccessibilityRole parentRole = parent->roleValue();
    if (parentRole == GroupRole)
      level++;
    else if (parentRole == TreeRole)
      break;
  }

  return level;
}
