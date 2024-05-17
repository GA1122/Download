void AXNodeObject::accessibilityChildrenFromAttribute(
    QualifiedName attr,
    AXObject::AXObjectVector& children) const {
  HeapVector<Member<Element>> elements;
  elementsFromAttribute(elements, attr);

  AXObjectCacheImpl& cache = axObjectCache();
  for (const auto& element : elements) {
    if (AXObject* child = cache.getOrCreate(element)) {
      if (child->accessibilityIsIgnored() && attr != aria_labelledbyAttr &&
          attr != aria_labeledbyAttr && attr != aria_describedbyAttr) {
        continue;
      }
      children.push_back(child);
    }
  }
}
