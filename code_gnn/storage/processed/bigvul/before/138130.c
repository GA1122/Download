String AXObject::ariaTextAlternative(bool recursive,
                                     bool inAriaLabelledByTraversal,
                                     AXObjectSet& visited,
                                     AXNameFrom& nameFrom,
                                     AXRelatedObjectVector* relatedObjects,
                                     NameSources* nameSources,
                                     bool* foundTextAlternative) const {
  String textAlternative;
  bool alreadyVisited = visited.contains(this);
  visited.insert(this);

  if (!inAriaLabelledByTraversal && isHiddenForTextAlternativeCalculation()) {
    *foundTextAlternative = true;
    return String();
  }

  if (!inAriaLabelledByTraversal && !alreadyVisited) {
    const QualifiedName& attr =
        hasAttribute(aria_labeledbyAttr) && !hasAttribute(aria_labelledbyAttr)
            ? aria_labeledbyAttr
            : aria_labelledbyAttr;
    nameFrom = AXNameFromRelatedElement;
    if (nameSources) {
      nameSources->push_back(NameSource(*foundTextAlternative, attr));
      nameSources->back().type = nameFrom;
    }

    const AtomicString& ariaLabelledby = getAttribute(attr);
    if (!ariaLabelledby.isNull()) {
      if (nameSources)
        nameSources->back().attributeValue = ariaLabelledby;

      AXObjectSet visitedCopy = visited;
      textAlternative = textFromAriaLabelledby(visitedCopy, relatedObjects);
      if (!textAlternative.isNull()) {
        if (nameSources) {
          NameSource& source = nameSources->back();
          source.type = nameFrom;
          source.relatedObjects = *relatedObjects;
          source.text = textAlternative;
          *foundTextAlternative = true;
        } else {
          *foundTextAlternative = true;
          return textAlternative;
        }
      } else if (nameSources) {
        nameSources->back().invalid = true;
      }
    }
  }

  nameFrom = AXNameFromAttribute;
  if (nameSources) {
    nameSources->push_back(NameSource(*foundTextAlternative, aria_labelAttr));
    nameSources->back().type = nameFrom;
  }
  const AtomicString& ariaLabel =
      getAOMPropertyOrARIAAttribute(AOMStringProperty::kLabel);
  if (!ariaLabel.isEmpty()) {
    textAlternative = ariaLabel;

    if (nameSources) {
      NameSource& source = nameSources->back();
      source.text = textAlternative;
      source.attributeValue = ariaLabel;
      *foundTextAlternative = true;
    } else {
      *foundTextAlternative = true;
      return textAlternative;
    }
  }

  return textAlternative;
}
