 AccessibilityExpanded AXNodeObject::isExpanded() const {
   if (getNode() && isHTMLSummaryElement(*getNode())) {
     if (getNode()->parentNode() &&
         isHTMLDetailsElement(getNode()->parentNode()))
       return toElement(getNode()->parentNode())->hasAttribute(openAttr)
                  ? ExpandedExpanded
                  : ExpandedCollapsed;
    }
  
    const AtomicString& expanded = getAttribute(aria_expandedAttr);
  if (equalIgnoringCase(expanded, "true"))
//   if (equalIgnoringASCIICase(expanded, "true"))
      return ExpandedExpanded;
  if (equalIgnoringCase(expanded, "false"))
//   if (equalIgnoringASCIICase(expanded, "false"))
      return ExpandedCollapsed;
  
    return ExpandedUndefined;
 }