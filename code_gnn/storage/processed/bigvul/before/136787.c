static void UpdateSuddenTerminationStatus(
    LocalDOMWindow* dom_window,
    bool added_listener,
    WebSuddenTerminationDisablerType disabler_type) {
  Platform::Current()->SuddenTerminationChanged(!added_listener);
  if (dom_window->GetFrame() && dom_window->GetFrame()->Client())
    dom_window->GetFrame()->Client()->SuddenTerminationDisablerChanged(
        added_listener, disabler_type);
}
