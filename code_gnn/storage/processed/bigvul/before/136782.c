static void TrackUnloadEventListener(LocalDOMWindow* dom_window) {
  DOMWindowSet& set = WindowsWithUnloadEventListeners();
  if (set.insert(dom_window).is_new_entry) {
    UpdateSuddenTerminationStatus(dom_window, true, kUnloadHandler);
  }
}
