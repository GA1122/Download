void RenderFrameImpl::OnExtendSelectionAndDelete(int before, int after) {
  if (!GetRenderWidget()->ShouldHandleImeEvent())
    return;

  DCHECK(!WebUserGestureIndicator::isProcessingUserGesture());

  ImeEventGuard guard(GetRenderWidget());
  blink::WebScopedUserGesture gesture_indicator;
  frame_->extendSelectionAndDelete(before, after);
}
