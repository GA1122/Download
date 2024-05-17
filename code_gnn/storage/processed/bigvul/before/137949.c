int AXLayoutObject::indexForVisiblePosition(
    const VisiblePosition& position) const {
  if (getLayoutObject() && getLayoutObject()->isTextControl()) {
    TextControlElement* textControl =
        toLayoutTextControl(getLayoutObject())->textControlElement();
    return textControl->indexForVisiblePosition(position);
  }

  if (!getNode())
    return 0;

  Position indexPosition = position.deepEquivalent();
  if (indexPosition.isNull())
    return 0;

  Range* range = Range::create(*getDocument());
  range->setStart(getNode(), 0, IGNORE_EXCEPTION_FOR_TESTING);
  range->setEnd(indexPosition, IGNORE_EXCEPTION_FOR_TESTING);

  return TextIterator::rangeLength(range->startPosition(),
                                   range->endPosition());
}
