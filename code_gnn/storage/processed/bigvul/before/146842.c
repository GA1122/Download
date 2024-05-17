bool ShouldInvalidateNodeListCachesForAttr<kNumNodeListInvalidationTypes>(
    const HeapHashSet<WeakMember<const LiveNodeListBase>>[],
    const QualifiedName&) {
  return false;
}
