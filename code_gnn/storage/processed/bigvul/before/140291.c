void Editor::applyParagraphStyleToSelection(StylePropertySet* style,
                                            InputEvent::InputType inputType) {
  if (!style || style->isEmpty() || !canEditRichly())
    return;

  applyParagraphStyle(style, inputType);
}
