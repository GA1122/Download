static DOMWindowSet& WindowsWithBeforeUnloadEventListeners() {
  DEFINE_STATIC_LOCAL(DOMWindowSet, windows_with_before_unload_event_listeners,
                      ());
  return windows_with_before_unload_event_listeners;
}
