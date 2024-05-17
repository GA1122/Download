AXObject::AXRange AXLayoutObject::textControlSelection() const {
  if (!getLayoutObject())
    return AXRange();

  LayoutObject* layout = nullptr;
  if (getLayoutObject()->isTextControl()) {
    layout = getLayoutObject();
  } else {
    Element* focusedElement = getDocument()->focusedElement();
    if (focusedElement && focusedElement->layoutObject() &&
        focusedElement->layoutObject()->isTextControl())
      layout = focusedElement->layoutObject();
  }

  if (!layout)
    return AXRange();

  AXObject* axObject = axObjectCache().getOrCreate(layout);
  if (!axObject || !axObject->isAXLayoutObject())
    return AXRange();

  VisibleSelection selection =
      layout->frame()->selection().computeVisibleSelectionInDOMTreeDeprecated();
  TextControlElement* textControl =
      toLayoutTextControl(layout)->textControlElement();
  ASSERT(textControl);
  int start = textControl->selectionStart();
  int end = textControl->selectionEnd();

  return AXRange(axObject, start, selection.visibleStart().affinity(), axObject,
                 end, selection.visibleEnd().affinity());
}
