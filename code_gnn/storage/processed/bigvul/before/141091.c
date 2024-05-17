Page* Document::GetPage() const {
  return frame_ ? frame_->GetPage() : nullptr;
}
