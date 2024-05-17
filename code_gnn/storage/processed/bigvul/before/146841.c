bool ShouldInvalidateNodeListCachesForAttr(
    const HeapHashSet<WeakMember<const LiveNodeListBase>> node_lists[],
    const QualifiedName& attr_name) {
  if (!node_lists[type].IsEmpty() &&
      LiveNodeListBase::ShouldInvalidateTypeOnAttributeChange(
          static_cast<NodeListInvalidationType>(type), attr_name))
    return true;
  return ShouldInvalidateNodeListCachesForAttr<type + 1>(node_lists, attr_name);
}
