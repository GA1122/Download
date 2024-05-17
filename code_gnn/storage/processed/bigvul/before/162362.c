std::unique_ptr<AudioOutputDelegate> CreateNoDelegate(
    AudioOutputDelegate::EventHandler* event_handler) {
  return nullptr;
}
