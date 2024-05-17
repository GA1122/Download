void WebLocalFrameImpl::DispatchMessageEventWithOriginCheck(
    const WebSecurityOrigin& intended_target_origin,
    const WebDOMEvent& event,
    bool has_user_gesture) {
  DCHECK(!event.IsNull());

  std::unique_ptr<UserGestureIndicator> gesture_indicator;
  if (!RuntimeEnabledFeatures::UserActivationV2Enabled() && has_user_gesture) {
    gesture_indicator = Frame::NotifyUserActivation(GetFrame());
    UserGestureIndicator::SetWasForwardedCrossProcess();
  }

  GetFrame()->DomWindow()->DispatchMessageEventWithOriginCheck(
      intended_target_origin.Get(), event,
      SourceLocation::Create(String(), 0, 0, nullptr));
}
