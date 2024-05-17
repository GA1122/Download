SVGElementSet& SVGElement::GetDependencyTraversalVisitedSet() {
  DEFINE_STATIC_LOCAL(Persistent<SVGElementSet>, invalidating_dependencies,
                      (MakeGarbageCollected<SVGElementSet>()));
  return *invalidating_dependencies;
}
