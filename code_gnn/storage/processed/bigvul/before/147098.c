void WebLocalFrameImpl::StopFinding(StopFindAction action) {
  bool clear_selection = action == kStopFindActionClearSelection;
  if (clear_selection)
    ExecuteCommand(WebString::FromUTF8("Unselect"));

  if (text_finder_) {
    if (!clear_selection)
      text_finder_->SetFindEndstateFocusAndSelection();
    text_finder_->StopFindingAndClearSelection();
  }

  if (action == kStopFindActionActivateSelection && IsFocused()) {
    WebDocument doc = GetDocument();
    if (!doc.IsNull()) {
      WebElement element = doc.FocusedElement();
      if (!element.IsNull())
        element.SimulateClick();
    }
  }
}
