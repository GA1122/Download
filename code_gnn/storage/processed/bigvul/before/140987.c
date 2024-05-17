Node* Document::Clone(Document& factory, CloneChildrenFlag flag) const {
  DCHECK_EQ(this, &factory)
      << "Document::Clone() doesn't support importNode mode.";
  Document* clone = CloneDocumentWithoutChildren();
  clone->CloneDataFromDocument(*this);
  if (flag == CloneChildrenFlag::kClone)
    clone->CloneChildNodesFrom(*this);
  return clone;
}
