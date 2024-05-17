bool Editor::selectionStartHasStyle(CSSPropertyID propertyID,
                                    const String& value) const {
  EditingStyle* styleToCheck = EditingStyle::create(propertyID, value);
  EditingStyle* styleAtStart =
      EditingStyleUtilities::createStyleAtSelectionStart(
          frame().selection().computeVisibleSelectionInDOMTreeDeprecated(),
          propertyID == CSSPropertyBackgroundColor, styleToCheck->style());
  return styleToCheck->triStateOfStyle(styleAtStart);
}
