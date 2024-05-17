void Document::UnregisterNodeList(const LiveNodeListBase* list) {
  DCHECK(node_lists_[list->InvalidationType()].Contains(list));
  node_lists_[list->InvalidationType()].erase(list);
  if (list->IsRootedAtTreeScope()) {
    DCHECK(lists_invalidated_at_document_.Contains(list));
    lists_invalidated_at_document_.erase(list);
  }
}
