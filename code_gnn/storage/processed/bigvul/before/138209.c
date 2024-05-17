String AXObject::textFromElements(bool inAriaLabelledbyTraversal,
                                  AXObjectSet& visited,
                                  HeapVector<Member<Element>>& elements,
                                  AXRelatedObjectVector* relatedObjects) const {
  StringBuilder accumulatedText;
  bool foundValidElement = false;
  AXRelatedObjectVector localRelatedObjects;

  for (const auto& element : elements) {
    AXObject* axElement = axObjectCache().getOrCreate(element);
    if (axElement) {
      foundValidElement = true;

      String result = recursiveTextAlternative(
          *axElement, inAriaLabelledbyTraversal, visited);
      localRelatedObjects.push_back(
          new NameSourceRelatedObject(axElement, result));
      if (!result.isEmpty()) {
        if (!accumulatedText.isEmpty())
          accumulatedText.append(' ');
        accumulatedText.append(result);
      }
    }
  }
  if (!foundValidElement)
    return String();
  if (relatedObjects)
    *relatedObjects = localRelatedObjects;
  return accumulatedText.toString();
}
