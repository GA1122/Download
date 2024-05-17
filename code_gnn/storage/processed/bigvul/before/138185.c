String AXObject::name(AXNameFrom& nameFrom,
                      AXObject::AXObjectVector* nameObjects) const {
  HeapHashSet<Member<const AXObject>> visited;
  AXRelatedObjectVector relatedObjects;
  String text = textAlternative(false, false, visited, nameFrom,
                                &relatedObjects, nullptr);

  AccessibilityRole role = roleValue();
  if (!getNode() || (!isHTMLBRElement(getNode()) && role != StaticTextRole &&
                     role != InlineTextBoxRole))
    text = collapseWhitespace(text);

  if (nameObjects) {
    nameObjects->clear();
    for (size_t i = 0; i < relatedObjects.size(); i++)
      nameObjects->push_back(relatedObjects[i]->object);
  }

  return text;
}
