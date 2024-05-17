void Editor::setBaseWritingDirection(WritingDirection direction) {
  Element* focusedElement = frame().document()->focusedElement();
  if (isTextControlElement(focusedElement)) {
    if (direction == NaturalWritingDirection)
      return;
    focusedElement->setAttribute(
        dirAttr, direction == LeftToRightWritingDirection ? "ltr" : "rtl");
    focusedElement->dispatchInputEvent();
    return;
  }

  MutableStylePropertySet* style =
      MutableStylePropertySet::create(HTMLQuirksMode);
  style->setProperty(
      CSSPropertyDirection,
      direction == LeftToRightWritingDirection
          ? "ltr"
          : direction == RightToLeftWritingDirection ? "rtl" : "inherit",
      false);
  applyParagraphStyleToSelection(
      style, InputEvent::InputType::FormatSetBlockTextDirection);
}
