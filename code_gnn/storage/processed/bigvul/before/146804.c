Document* Document::ParentDocument() const {
  if (!frame_)
    return 0;
  Frame* parent = frame_->Tree().Parent();
  if (!parent || !parent->IsLocalFrame())
    return 0;
  return ToLocalFrame(parent)->GetDocument();
}
