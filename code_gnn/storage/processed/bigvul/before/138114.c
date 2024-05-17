String AXNodeObject::textAlternative(bool recursive,
                                     bool inAriaLabelledByTraversal,
                                     AXObjectSet& visited,
                                     AXNameFrom& nameFrom,
                                     AXRelatedObjectVector* relatedObjects,
                                     NameSources* nameSources) const {
  if (nameSources)
    ASSERT(relatedObjects);

  bool foundTextAlternative = false;

  if (!getNode() && !getLayoutObject())
    return String();

  String textAlternative = ariaTextAlternative(
      recursive, inAriaLabelledByTraversal, visited, nameFrom, relatedObjects,
      nameSources, &foundTextAlternative);
  if (foundTextAlternative && !nameSources)
    return textAlternative;

  if (recursive && !inAriaLabelledByTraversal && isControl() && !isButton()) {
    if (isTextControl())
      return text();

    if (isRange()) {
      const AtomicString& ariaValuetext =
          getAOMPropertyOrARIAAttribute(AOMStringProperty::kValueText);
      if (!ariaValuetext.isNull())
        return ariaValuetext.getString();
      return String::number(valueForRange());
    }

    return stringValue();
  }

  textAlternative = nativeTextAlternative(visited, nameFrom, relatedObjects,
                                          nameSources, &foundTextAlternative);
  if (!textAlternative.isEmpty() && !nameSources)
    return textAlternative;

  if (recursive || nameFromContents()) {
    nameFrom = AXNameFromContents;
    if (nameSources) {
      nameSources->push_back(NameSource(foundTextAlternative));
      nameSources->back().type = nameFrom;
    }

    Node* node = this->getNode();
    if (node && node->isTextNode())
      textAlternative = toText(node)->wholeText();
    else if (isHTMLBRElement(node))
      textAlternative = String("\n");
    else
      textAlternative = textFromDescendants(visited, false);

    if (!textAlternative.isEmpty()) {
      if (nameSources) {
        foundTextAlternative = true;
        nameSources->back().text = textAlternative;
      } else {
        return textAlternative;
      }
    }
  }

  nameFrom = AXNameFromTitle;
  if (nameSources) {
    nameSources->push_back(NameSource(foundTextAlternative, titleAttr));
    nameSources->back().type = nameFrom;
  }
  const AtomicString& title = getAttribute(titleAttr);
  if (!title.isEmpty()) {
    textAlternative = title;
    if (nameSources) {
      foundTextAlternative = true;
      nameSources->back().text = textAlternative;
    } else {
      return textAlternative;
    }
  }

  nameFrom = AXNameFromUninitialized;

  if (nameSources && foundTextAlternative) {
    for (size_t i = 0; i < nameSources->size(); ++i) {
      if (!(*nameSources)[i].text.isNull() && !(*nameSources)[i].superseded) {
        NameSource& nameSource = (*nameSources)[i];
        nameFrom = nameSource.type;
        if (!nameSource.relatedObjects.isEmpty())
          *relatedObjects = nameSource.relatedObjects;
        return nameSource.text;
      }
    }
  }

  return String();
}
