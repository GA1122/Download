void AddElementToDocumentMap(HTMLMediaElement* element, Document* document) {
  DocumentElementSetMap& map = DocumentToElementSetMap();
  WeakMediaElementSet* set = nullptr;
  auto it = map.find(document);
  if (it == map.end()) {
    set = new WeakMediaElementSet;
    map.insert(document, set);
  } else {
    set = it->value;
  }
  set->insert(element);
}
