static bool isRequiredOwnedElement(AXObject* parent,
                                   AccessibilityRole currentRole,
                                   HTMLElement* currentElement) {
  Node* parentNode = parent->getNode();
  if (!parentNode || !parentNode->isHTMLElement())
    return false;

  if (currentRole == ListItemRole)
    return isListElement(parentNode);
  if (currentRole == ListMarkerRole)
    return isHTMLLIElement(*parentNode);
  if (currentRole == MenuItemCheckBoxRole || currentRole == MenuItemRole ||
      currentRole == MenuItemRadioRole)
    return isHTMLMenuElement(*parentNode);

  if (!currentElement)
    return false;
  if (isHTMLTableCellElement(*currentElement))
    return isHTMLTableRowElement(*parentNode);
  if (isHTMLTableRowElement(*currentElement))
    return isHTMLTableSectionElement(toHTMLElement(*parentNode));

  return false;
}
