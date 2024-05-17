Document* Document::ParentDocument() const {
  if (!frame_)
    return nullptr;
  Frame* parent = frame_->Tree().Parent();
  if (!parent || !parent->IsLocalFrame())
    return nullptr;
  return ToLocalFrame(parent)->GetDocument();
}
