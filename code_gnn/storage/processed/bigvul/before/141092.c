const FeaturePolicy* Document::GetParentFeaturePolicy() const {
  if (frame_ && !frame_->IsMainFrame())
    return frame_->Tree().Parent()->GetSecurityContext()->GetFeaturePolicy();
  return nullptr;
}
