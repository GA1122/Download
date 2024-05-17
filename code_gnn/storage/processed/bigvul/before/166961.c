void CSSStyleSheet::ClearOwnerNode() {
  DidMutate();
  if (owner_node_)
    contents_->UnregisterClient(this);
  owner_node_ = nullptr;
}
