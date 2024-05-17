bool InputMethodController::CommitText(
    const String& text,
    const Vector<CompositionUnderline>& underlines,
    int relative_caret_position) {
  if (HasComposition()) {
    return ReplaceCompositionAndMoveCaret(text, relative_caret_position,
                                          underlines);
  }

  return InsertTextAndMoveCaret(text, relative_caret_position, underlines);
}
