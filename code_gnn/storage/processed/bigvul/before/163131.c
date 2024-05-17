void HTMLFrameOwnerElement::ClearContentFrame() {
  if (!content_frame_)
    return;

  DCHECK_EQ(content_frame_->Owner(), this);
  content_frame_ = nullptr;

  for (ContainerNode* node = this; node; node = node->ParentOrShadowHostNode())
    node->DecrementConnectedSubframeCount();
}
