DocumentElementSetMap& DocumentToElementSetMap() {
  DEFINE_STATIC_LOCAL(Persistent<DocumentElementSetMap>, map,
                      (MakeGarbageCollected<DocumentElementSetMap>()));
  return *map;
}
