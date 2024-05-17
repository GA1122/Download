 void WebLocalFrameImpl::NotifyUserActivation() {
  Frame::NotifyUserActivation(GetFrame(), UserGestureToken::kNewGesture);
}
