bool SelectionController::SelectClosestWordFromMouseEvent(
    const MouseEventWithHitTestResults& result) {
  if (!mouse_down_may_start_select_)
    return false;

  AppendTrailingWhitespace append_trailing_whitespace =
      (result.Event().click_count == 2 &&
       frame_->GetEditor().IsSelectTrailingWhitespaceEnabled())
          ? AppendTrailingWhitespace::kShouldAppend
          : AppendTrailingWhitespace::kDontAppend;

  DCHECK(!frame_->GetDocument()->NeedsLayoutTreeUpdate());

  return SelectClosestWordFromHitTestResult(
      result.GetHitTestResult(), append_trailing_whitespace,
      result.Event().FromTouch() ? SelectInputEventType::kTouch
                                 : SelectInputEventType::kMouse);
}
