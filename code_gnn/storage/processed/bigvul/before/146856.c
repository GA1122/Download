void Document::UnregisterNodeListWithIdNameCache(const LiveNodeListBase* list) {
  DCHECK(node_lists_[kInvalidateOnIdNameAttrChange].Contains(list));
  node_lists_[kInvalidateOnIdNameAttrChange].erase(list);
}
