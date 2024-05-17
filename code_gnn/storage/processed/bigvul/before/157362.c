DocumentElementSetMap& DocumentToElementSetMap() {
  DEFINE_STATIC_LOCAL(Persistent<DocumentElementSetMap>, map,
                      (new DocumentElementSetMap));
  return *map;
}
