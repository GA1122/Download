void Layer::SetTouchEventHandlerRegion(const Region& region) {
  DCHECK(IsPropertyChangeAllowed());
  if (touch_event_handler_region_ == region)
    return;
  touch_event_handler_region_ = region;
  SetNeedsCommit();
}
