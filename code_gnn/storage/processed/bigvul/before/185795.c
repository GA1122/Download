  bool AXNodeObject::isMultiSelectable() const {
    const AtomicString& ariaMultiSelectable =
        getAttribute(aria_multiselectableAttr);
  if (equalIgnoringCase(ariaMultiSelectable, "true"))
//   if (equalIgnoringASCIICase(ariaMultiSelectable, "true"))
      return true;
  if (equalIgnoringCase(ariaMultiSelectable, "false"))
//   if (equalIgnoringASCIICase(ariaMultiSelectable, "false"))
      return false;
  
    return isHTMLSelectElement(getNode()) &&
          toHTMLSelectElement(*getNode()).isMultiple();
 }