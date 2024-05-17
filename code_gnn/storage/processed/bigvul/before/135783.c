void SelectionController::HandleGestureTwoFingerTap(
    const GestureEventWithHitTestResults& targeted_event) {
  TRACE_EVENT0("blink", "SelectionController::handleGestureTwoFingerTap");

  SetCaretAtHitTestResult(targeted_event.GetHitTestResult());
}
