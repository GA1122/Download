const ParsedFeaturePolicy Document::GetOwnerContainerPolicy() const {
  if (frame_ && frame_->Owner())
    return frame_->Owner()->ContainerPolicy();
  return ParsedFeaturePolicy();
}
