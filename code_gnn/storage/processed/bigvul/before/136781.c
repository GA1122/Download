static void TrackBeforeUnloadEventListener(LocalDOMWindow* dom_window) {
  DOMWindowSet& set = WindowsWithBeforeUnloadEventListeners();
  if (set.insert(dom_window).is_new_entry) {
    UpdateSuddenTerminationStatus(dom_window, true, kBeforeUnloadHandler);
  }
}
