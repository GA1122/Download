bool Document::ShouldInvalidateNodeListCaches(
    const QualifiedName* attr_name) const {
  if (attr_name) {
    return ShouldInvalidateNodeListCachesForAttr<
        kDoNotInvalidateOnAttributeChanges + 1>(node_lists_, *attr_name);
  }

  for (int type = 0; type < kNumNodeListInvalidationTypes; ++type) {
    if (!node_lists_[type].IsEmpty())
      return true;
  }

  return false;
}
