bool CSSStyleSheet::IsAlternate() const {
  if (owner_node_) {
    return owner_node_->IsElementNode() &&
           ToElement(owner_node_)->getAttribute(relAttr).Contains("alternate");
  }
  return alternate_from_constructor_;
}
