void LocalFrame::SetInheritedEffectiveTouchAction(TouchAction touch_action) {
  if (inherited_effective_touch_action_ == touch_action)
    return;
  inherited_effective_touch_action_ = touch_action;
  if (GetDocument()->documentElement()) {
    GetDocument()->documentElement()->SetNeedsStyleRecalc(
        kSubtreeStyleChange,
        StyleChangeReasonForTracing::Create(
            StyleChangeReason::kInheritedStyleChangeFromParentFrame));
  }
}
