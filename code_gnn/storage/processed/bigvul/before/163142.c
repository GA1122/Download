void HTMLFrameOwnerElement::SetContentFrame(Frame& frame) {
  DCHECK(!content_frame_ || content_frame_->Owner() != this);
  DCHECK(isConnected());
  content_frame_ = &frame;

  for (ContainerNode* node = this; node; node = node->ParentOrShadowHostNode())
    node->IncrementConnectedSubframeCount();
}
