bool AXNodeObject::nameFromLabelElement() const {

  if (!getNode() && !getLayoutObject())
    return false;

  if (isHiddenForTextAlternativeCalculation())
    return false;

  HeapVector<Member<Element>> elements;
  ariaLabelledbyElementVector(elements);
  if (elements.size() > 0)
    return false;

  const AtomicString& ariaLabel =
      getAOMPropertyOrARIAAttribute(AOMStringProperty::kLabel);
  if (!ariaLabel.isEmpty())
    return false;

  HTMLElement* htmlElement = nullptr;
  if (getNode()->isHTMLElement())
    htmlElement = toHTMLElement(getNode());
  if (htmlElement && isLabelableElement(htmlElement)) {
    if (toLabelableElement(htmlElement)->labels() &&
        toLabelableElement(htmlElement)->labels()->length() > 0)
      return true;
  }

  return false;
}
