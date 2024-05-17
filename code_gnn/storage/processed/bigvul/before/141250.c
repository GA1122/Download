bool ShouldInvalidateNodeListCachesForAttr<kNumNodeListInvalidationTypes>(
    const LiveNodeListRegistry&,
    const QualifiedName&) {
  return false;
}
