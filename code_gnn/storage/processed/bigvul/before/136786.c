static void UntrackUnloadEventListener(LocalDOMWindow* dom_window) {
  DOMWindowSet& set = WindowsWithUnloadEventListeners();
  DOMWindowSet::iterator it = set.find(dom_window);
  if (it == set.end())
    return;
  if (set.erase(it)) {
    UpdateSuddenTerminationStatus(dom_window, false, kUnloadHandler);
  }
}
