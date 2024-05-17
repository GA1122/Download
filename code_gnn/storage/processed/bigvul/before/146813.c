void Document::RegisterNodeList(const LiveNodeListBase* list) {
  DCHECK(!node_lists_[list->InvalidationType()].Contains(list));
  node_lists_[list->InvalidationType()].insert(list);
  LiveNodeListBaseWriteBarrier(this, list);
  if (list->IsRootedAtTreeScope())
    lists_invalidated_at_document_.insert(list);
}
