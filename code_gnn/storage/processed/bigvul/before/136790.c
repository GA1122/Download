static DOMWindowSet& WindowsWithUnloadEventListeners() {
  DEFINE_STATIC_LOCAL(DOMWindowSet, windows_with_unload_event_listeners, ());
  return windows_with_unload_event_listeners;
}
