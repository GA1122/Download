TriState Editor::SelectionHasStyle(CSSPropertyID property_id,
                                   const String& value) const {
  return EditingStyle::Create(property_id, value)
      ->TriStateOfStyle(
          GetFrame().Selection().ComputeVisibleSelectionInDOMTreeDeprecated());
}
