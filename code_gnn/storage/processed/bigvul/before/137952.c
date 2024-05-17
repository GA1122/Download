bool AXLayoutObject::isFocused() const {
  if (!getDocument())
    return false;

  Element* focusedElement = getDocument()->focusedElement();
  if (!focusedElement)
    return false;
  AXObject* focusedObject = axObjectCache().getOrCreate(focusedElement);
  if (!focusedObject || !focusedObject->isAXLayoutObject())
    return false;

  if (focusedObject == this ||
      (roleValue() == WebAreaRole &&
       getDocument()->frame()->selection().isFocusedAndActive()))
    return true;

  return false;
}
