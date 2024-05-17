bool Editor::SelectionStartHasStyle(CSSPropertyID property_id,
                                    const String& value) const {
  EditingStyle* style_to_check = EditingStyle::Create(property_id, value);
  EditingStyle* style_at_start =
      EditingStyleUtilities::CreateStyleAtSelectionStart(
          GetFrame().Selection().ComputeVisibleSelectionInDOMTreeDeprecated(),
          property_id == CSSPropertyBackgroundColor, style_to_check->Style());
  return style_to_check->TriStateOfStyle(style_at_start);
}
