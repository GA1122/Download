TriState Editor::selectionHasStyle(CSSPropertyID propertyID,
                                   const String& value) const {
  return EditingStyle::create(propertyID, value)
      ->triStateOfStyle(
          frame().selection().computeVisibleSelectionInDOMTreeDeprecated());
}
