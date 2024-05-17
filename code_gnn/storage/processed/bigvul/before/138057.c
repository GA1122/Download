bool AXNodeObject::isClickable() const {
  if (getNode()) {
    if (getNode()->isElementNode() &&
        toElement(getNode())->isDisabledFormControl())
      return false;

    if (getNode()->hasEventListeners(EventTypeNames::mouseup) ||
        getNode()->hasEventListeners(EventTypeNames::mousedown) ||
        getNode()->hasEventListeners(EventTypeNames::click) ||
        getNode()->hasEventListeners(EventTypeNames::DOMActivate))
      return true;
  }

  return AXObject::isClickable();
}
