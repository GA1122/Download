void HWNDMessageHandler::HandleTouchEvents(const TouchEvents& touch_events) {
  base::WeakPtr<HWNDMessageHandler> ref(weak_factory_.GetWeakPtr());
  for (size_t i = 0; i < touch_events.size() && ref; ++i)
    delegate_->HandleTouchEvent(touch_events[i]);
}
