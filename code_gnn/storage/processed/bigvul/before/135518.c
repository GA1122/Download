void Editor::ApplyStyleToSelection(StylePropertySet* style,
                                   InputEvent::InputType input_type) {
  if (!style || style->IsEmpty() || !CanEditRichly())
    return;

  ApplyStyle(style, input_type);
}
