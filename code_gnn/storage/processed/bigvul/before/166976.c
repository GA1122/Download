Document* CSSStyleSheet::OwnerDocument() const {
  const CSSStyleSheet* root = this;
  while (root->parentStyleSheet())
    root = root->parentStyleSheet();
  return root->ownerNode() ? &root->ownerNode()->GetDocument() : nullptr;
}
