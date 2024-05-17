void AXObject::elementsFromAttribute(HeapVector<Member<Element>>& elements,
                                     const QualifiedName& attribute) const {
  Vector<String> ids;
  tokenVectorFromAttribute(ids, attribute);
  if (ids.isEmpty())
    return;

  TreeScope& scope = getNode()->treeScope();
  for (const auto& id : ids) {
    if (Element* idElement = scope.getElementById(AtomicString(id)))
      elements.push_back(idElement);
  }
}
