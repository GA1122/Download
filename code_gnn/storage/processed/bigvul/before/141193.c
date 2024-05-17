void Document::RegisterNodeList(const LiveNodeListBase* list) {
  node_lists_.Add(list, list->InvalidationType());
  LiveNodeListBaseWriteBarrier(this, list);
  if (list->IsRootedAtTreeScope())
    lists_invalidated_at_document_.insert(list);
}
