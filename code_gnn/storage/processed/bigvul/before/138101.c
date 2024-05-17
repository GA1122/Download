String AXNodeObject::placeholder(AXNameFrom nameFrom) const {
  if (nameFrom == AXNameFromPlaceholder)
    return String();

  Node* node = getNode();
  if (!node || !node->isHTMLElement())
    return String();

  String nativePlaceholder = placeholderFromNativeAttribute();
  if (!nativePlaceholder.isEmpty())
    return nativePlaceholder;

  const AtomicString& ariaPlaceholder =
      toHTMLElement(node)->fastGetAttribute(aria_placeholderAttr);
  if (!ariaPlaceholder.isEmpty())
    return ariaPlaceholder;

  return String();
}
