void Editor::SetBaseWritingDirection(WritingDirection direction) {
  Element* focused_element = GetFrame().GetDocument()->FocusedElement();
  if (IsTextControlElement(focused_element)) {
    if (direction == NaturalWritingDirection)
      return;
    focused_element->setAttribute(
        dirAttr, direction == LeftToRightWritingDirection ? "ltr" : "rtl");
    focused_element->DispatchInputEvent();
    return;
  }

  MutableStylePropertySet* style =
      MutableStylePropertySet::Create(kHTMLQuirksMode);
  style->SetProperty(
      CSSPropertyDirection,
      direction == LeftToRightWritingDirection
          ? "ltr"
          : direction == RightToLeftWritingDirection ? "rtl" : "inherit",
      false);
  ApplyParagraphStyleToSelection(
      style, InputEvent::InputType::kFormatSetBlockTextDirection);
}
