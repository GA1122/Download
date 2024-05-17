FrameLoader::InsecureNavigationsToUpgrade() const {
  DCHECK(frame_);
  Frame* parent_frame = frame_->Tree().Parent();
  if (!parent_frame)
    return nullptr;

  return parent_frame->GetSecurityContext()->InsecureNavigationsToUpgrade();
}
