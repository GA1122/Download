String AXObject::textFromAriaLabelledby(
    AXObjectSet& visited,
    AXRelatedObjectVector* relatedObjects) const {
  HeapVector<Member<Element>> elements;
  ariaLabelledbyElementVector(elements);
  return textFromElements(true, visited, elements, relatedObjects);
}
