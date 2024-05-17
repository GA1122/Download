void Document::ChildrenChanged(const ChildrenChange& change) {
  ContainerNode::ChildrenChanged(change);
  document_element_ = ElementTraversal::FirstWithin(*this);

  if (document_element_ && !IsHTMLDocument())
    BeginLifecycleUpdatesIfRenderingReady();
}
