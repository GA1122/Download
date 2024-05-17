String AXObject::textFromAriaDescribedby(
    AXRelatedObjectVector* relatedObjects) const {
  AXObjectSet visited;
  HeapVector<Member<Element>> elements;
  elementsFromAttribute(elements, aria_describedbyAttr);
  return textFromElements(true, visited, elements, relatedObjects);
}
