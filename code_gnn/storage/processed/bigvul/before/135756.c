bool InputMethodController::ReplaceCompositionAndMoveCaret(
    const String& text,
    int relative_caret_position,
    const Vector<CompositionUnderline>& underlines) {
  Element* root_editable_element =
      GetFrame()
          .Selection()
          .ComputeVisibleSelectionInDOMTreeDeprecated()
          .RootEditableElement();
  if (!root_editable_element)
    return false;
  DCHECK(HasComposition());
  PlainTextRange composition_range =
      PlainTextRange::Create(*root_editable_element, *composition_range_);
  if (composition_range.IsNull())
    return false;
  int text_start = composition_range.Start();

  if (!ReplaceComposition(text))
    return false;

  GetDocument().UpdateStyleAndLayoutIgnorePendingStylesheets();

  AddCompositionUnderlines(underlines, root_editable_element, text_start);

  int absolute_caret_position = ComputeAbsoluteCaretPosition(
      text_start, text.length(), relative_caret_position);
  return MoveCaret(absolute_caret_position);
}
