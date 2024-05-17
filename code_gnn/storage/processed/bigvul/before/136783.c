static void UntrackAllBeforeUnloadEventListeners(LocalDOMWindow* dom_window) {
  DOMWindowSet& set = WindowsWithBeforeUnloadEventListeners();
  DOMWindowSet::iterator it = set.find(dom_window);
  if (it == set.end())
    return;
  set.RemoveAll(it);
  UpdateSuddenTerminationStatus(dom_window, false, kBeforeUnloadHandler);
}
