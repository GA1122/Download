void InputMethodController::AddCompositionUnderlines(
    const Vector<CompositionUnderline>& underlines,
    ContainerNode* base_element,
    unsigned offset_in_plain_chars) {
  for (const auto& underline : underlines) {
    unsigned underline_start = offset_in_plain_chars + underline.StartOffset();
    unsigned underline_end = offset_in_plain_chars + underline.EndOffset();

    EphemeralRange ephemeral_line_range =
        PlainTextRange(underline_start, underline_end)
            .CreateRange(*base_element);
    if (ephemeral_line_range.IsNull())
      continue;

    GetDocument().Markers().AddCompositionMarker(
        ephemeral_line_range, underline.GetColor(),
        underline.Thick() ? StyleableMarker::Thickness::kThick
                          : StyleableMarker::Thickness::kThin,
        underline.BackgroundColor());
  }
}
