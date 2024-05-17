static void UntrackAllUnloadEventListeners(LocalDOMWindow* dom_window) {
  DOMWindowSet& set = WindowsWithUnloadEventListeners();
  DOMWindowSet::iterator it = set.find(dom_window);
  if (it == set.end())
    return;
  set.RemoveAll(it);
  UpdateSuddenTerminationStatus(dom_window, false, kUnloadHandler);
}
