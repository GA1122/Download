void HTMLInputElement::ChildrenChanged(const ChildrenChange& change) {
  EnsureUserAgentShadowRoot();
  ContainerNode::ChildrenChanged(change);
}
