static HeapHashSet<WeakMember<SVGElement>>& EmptyInstances() {
  DEFINE_STATIC_LOCAL(
      Persistent<HeapHashSet<WeakMember<SVGElement>>>, empty_instances,
      (MakeGarbageCollected<HeapHashSet<WeakMember<SVGElement>>>()));
  return *empty_instances;
}
