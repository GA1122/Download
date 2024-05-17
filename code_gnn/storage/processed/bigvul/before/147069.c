void WebLocalFrameImpl::SendOrientationChangeEvent() {
  if (!GetFrame())
    return;

  if (ScreenOrientationController::From(*GetFrame()))
    ScreenOrientationController::From(*GetFrame())->NotifyOrientationChanged();

  if (RuntimeEnabledFeatures::OrientationEventEnabled() &&
      GetFrame()->DomWindow())
    GetFrame()->DomWindow()->SendOrientationChangeEvent();
}
