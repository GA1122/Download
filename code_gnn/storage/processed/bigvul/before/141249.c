bool Document::ShouldInvalidateNodeListCaches(
    const QualifiedName* attr_name) const {
  if (attr_name) {
    return ShouldInvalidateNodeListCachesForAttr<
        kDoNotInvalidateOnAttributeChanges + 1>(node_lists_, *attr_name);
  }

  return !node_lists_.IsEmpty();
}
