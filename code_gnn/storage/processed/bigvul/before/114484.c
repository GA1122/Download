void RenderingHelper::PlatformUnInitialize() {
  for (size_t i = 0; i < windows_.size(); ++i) {
    DestroyWindow(windows_[i]);
  }
  windows_.clear();
}
