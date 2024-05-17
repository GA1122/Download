bool InputMethodController::InsertTextAndMoveCaret(
    const String& text,
    int relative_caret_position,
    const Vector<CompositionUnderline>& underlines) {
  PlainTextRange selection_range = GetSelectionOffsets();
  if (selection_range.IsNull())
    return false;
  int text_start = selection_range.Start();

  if (!InsertText(text))
    return false;
  Element* root_editable_element =
      GetFrame()
          .Selection()
          .ComputeVisibleSelectionInDOMTreeDeprecated()
          .RootEditableElement();
  if (root_editable_element) {
    AddCompositionUnderlines(underlines, root_editable_element, text_start);
  }

  int absolute_caret_position = ComputeAbsoluteCaretPosition(
      text_start, text.length(), relative_caret_position);
  return MoveCaret(absolute_caret_position);
}
