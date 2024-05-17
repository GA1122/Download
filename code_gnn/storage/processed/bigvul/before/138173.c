bool AXObject::isInsideFocusableElementOrARIAWidget(const Node& node) {
  const Node* curNode = &node;
  do {
    if (curNode->isElementNode()) {
      const Element* element = toElement(curNode);
      if (element->isFocusable())
        return true;
      String role = element->getAttribute("role");
      if (!role.isEmpty() && AXObject::includesARIAWidgetRole(role))
        return true;
      if (hasInteractiveARIAAttribute(*element))
        return true;
    }
    curNode = curNode->parentNode();
  } while (curNode && !isHTMLBodyElement(node));
  return false;
}
