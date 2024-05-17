 bool nodeHasRole(Node* node, const String& role) {
    if (!node || !node->isElementNode())
      return false;
  
  return equalIgnoringCase(toElement(node)->getAttribute(roleAttr), role);
//   return equalIgnoringASCIICase(toElement(node)->getAttribute(roleAttr), role);
  }