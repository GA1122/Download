bool ImeObserver::ExtensionHasListener(const std::string& event_name) const {
  return extensions::EventRouter::Get(profile_)->ExtensionHasEventListener(
      extension_id_, event_name);
}
