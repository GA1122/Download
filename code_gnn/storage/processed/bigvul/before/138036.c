AXObject* AXNodeObject::correspondingControlForLabelElement() const {
  HTMLLabelElement* labelElement = labelElementContainer();
  if (!labelElement)
    return 0;

  HTMLElement* correspondingControl = labelElement->control();
  if (!correspondingControl)
    return 0;

  if (correspondingControl->layoutObject() &&
      !correspondingControl->layoutObject()->parent())
    return 0;

  return axObjectCache().getOrCreate(correspondingControl);
}
