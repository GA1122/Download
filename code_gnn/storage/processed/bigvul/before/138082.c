static bool isPresentationalInTable(AXObject* parent,
                                    HTMLElement* currentElement) {
  if (!currentElement)
    return false;

  Node* parentNode = parent->getNode();
  if (!parentNode || !parentNode->isHTMLElement())
    return false;

  if (isHTMLTableCellElement(*currentElement) &&
      isHTMLTableRowElement(*parentNode))
    return parent->hasInheritedPresentationalRole();

  if (isHTMLTableRowElement(*currentElement) &&
      isHTMLTableSectionElement(toHTMLElement(*parentNode))) {
    AXObject* tableObject = parent->parentObject();
    Node* tableNode = tableObject ? tableObject->getNode() : 0;
    return isHTMLTableElement(tableNode) &&
           tableObject->hasInheritedPresentationalRole();
  }
  return false;
}
