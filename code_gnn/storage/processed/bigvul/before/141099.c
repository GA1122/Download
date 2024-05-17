Settings* Document::GetSettings() const {
  return frame_ ? frame_->GetSettings() : nullptr;
}
