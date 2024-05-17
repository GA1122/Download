String Editor::selectionStartCSSPropertyValue(CSSPropertyID propertyID) {
  EditingStyle* selectionStyle =
      EditingStyleUtilities::createStyleAtSelectionStart(
          frame().selection().computeVisibleSelectionInDOMTreeDeprecated(),
          propertyID == CSSPropertyBackgroundColor);
  if (!selectionStyle || !selectionStyle->style())
    return String();

  if (propertyID == CSSPropertyFontSize)
    return String::number(selectionStyle->legacyFontSize(frame().document()));
  return selectionStyle->style()->getPropertyValue(propertyID);
}
