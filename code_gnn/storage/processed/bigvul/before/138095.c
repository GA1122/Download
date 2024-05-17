Element* AXNodeObject::mouseButtonListener() const {
  Node* node = this->getNode();
  if (!node)
    return 0;

  if (!node->isElementNode())
    node = node->parentElement();

  if (!node)
    return 0;

  for (Element* element = toElement(node); element;
       element = element->parentElement()) {
    if (isHTMLBodyElement(element))
      break;

    if (element->hasEventListeners(EventTypeNames::click) ||
        element->hasEventListeners(EventTypeNames::mousedown) ||
        element->hasEventListeners(EventTypeNames::mouseup) ||
        element->hasEventListeners(EventTypeNames::DOMActivate))
      return element;
  }

  return 0;
}
