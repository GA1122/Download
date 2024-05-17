void removeElementFromDocumentMap(HTMLMediaElement* element,
                                  Document* document) {
  DocumentElementSetMap& map = documentToElementSetMap();
  auto it = map.find(document);
  DCHECK(it != map.end());
  WeakMediaElementSet* set = it->value;
  set->remove(element);
  if (set->isEmpty())
    map.remove(it);
}
