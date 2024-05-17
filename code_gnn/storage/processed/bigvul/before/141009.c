void Document::DetachNodeIterator(NodeIterator* ni) {
  node_iterators_.erase(ni);
}
