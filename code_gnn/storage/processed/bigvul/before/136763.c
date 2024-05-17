void LocalDOMWindow::PostMessageTimerFired(PostMessageTimer* timer) {
  if (!IsCurrentlyDisplayedInFrame())
    return;

  MessageEvent* event = timer->Event();

  UserGestureToken* token = timer->GetUserGestureToken();
  std::unique_ptr<UserGestureIndicator> gesture_indicator;
  if (!RuntimeEnabledFeatures::UserActivationV2Enabled() && token &&
      token->HasGestures() && document())
    gesture_indicator = Frame::NotifyUserActivation(document()->GetFrame());

  event->EntangleMessagePorts(document());

  DispatchMessageEventWithOriginCheck(timer->TargetOrigin(), event,
                                      timer->TakeLocation());
}
