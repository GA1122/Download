std::unique_ptr<AudioInputDelegate> CreateNoDelegate(
    AudioInputDelegate::EventHandler* event_handler) {
  return nullptr;
}
