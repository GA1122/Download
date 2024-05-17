Document* Document::ParentDocument() const {
  if (!frame_)
    return nullptr;
  auto* parent_local_frame = DynamicTo<LocalFrame>(frame_->Tree().Parent());
  if (!parent_local_frame)
    return nullptr;
  return parent_local_frame->GetDocument();
}
