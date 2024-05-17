void Editor::ApplyParagraphStyleToSelection(StylePropertySet* style,
                                            InputEvent::InputType input_type) {
  if (!style || style->IsEmpty() || !CanEditRichly())
    return;

  ApplyParagraphStyle(style, input_type);
}
