void SelectionController::HandleGestureLongTap(
    const GestureEventWithHitTestResults& targeted_event) {
  TRACE_EVENT0("blink", "SelectionController::handleGestureLongTap");

  SetCaretAtHitTestResult(targeted_event.GetHitTestResult());
}
