String AXListBoxOption::textAlternative(bool recursive,
                                        bool inAriaLabelledByTraversal,
                                        AXObjectSet& visited,
                                        AXNameFrom& nameFrom,
                                        AXRelatedObjectVector* relatedObjects,
                                        NameSources* nameSources) const {
  if (nameSources)
    ASSERT(relatedObjects);

  if (!getNode())
    return String();

  bool foundTextAlternative = false;
  String textAlternative = ariaTextAlternative(
      recursive, inAriaLabelledByTraversal, visited, nameFrom, relatedObjects,
      nameSources, &foundTextAlternative);
  if (foundTextAlternative && !nameSources)
    return textAlternative;

  nameFrom = AXNameFromContents;
  textAlternative = toHTMLOptionElement(getNode())->displayLabel();
  if (nameSources) {
    nameSources->push_back(NameSource(foundTextAlternative));
    nameSources->back().type = nameFrom;
    nameSources->back().text = textAlternative;
    foundTextAlternative = true;
  }

  return textAlternative;
}
