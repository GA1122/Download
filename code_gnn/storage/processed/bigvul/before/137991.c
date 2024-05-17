String AXLayoutObject::textAlternative(bool recursive,
                                       bool inAriaLabelledByTraversal,
                                       AXObjectSet& visited,
                                       AXNameFrom& nameFrom,
                                       AXRelatedObjectVector* relatedObjects,
                                       NameSources* nameSources) const {
  if (m_layoutObject) {
    String textAlternative;
    bool foundTextAlternative = false;

    if (m_layoutObject->isBR()) {
      textAlternative = String("\n");
      foundTextAlternative = true;
    } else if (m_layoutObject->isText() &&
               (!recursive || !m_layoutObject->isCounter())) {
      LayoutText* layoutText = toLayoutText(m_layoutObject);
      String result = layoutText->plainText();
      if (!result.isEmpty() || layoutText->isAllCollapsibleWhitespace())
        textAlternative = result;
      else
        textAlternative = layoutText->text();
      foundTextAlternative = true;
    } else if (m_layoutObject->isListMarker() && !recursive) {
      textAlternative = toLayoutListMarker(m_layoutObject)->text();
      foundTextAlternative = true;
    }

    if (foundTextAlternative) {
      nameFrom = AXNameFromContents;
      if (nameSources) {
        nameSources->push_back(NameSource(false));
        nameSources->back().type = nameFrom;
        nameSources->back().text = textAlternative;
      }
      return textAlternative;
    }
  }

  return AXNodeObject::textAlternative(recursive, inAriaLabelledByTraversal,
                                       visited, nameFrom, relatedObjects,
                                       nameSources);
}
