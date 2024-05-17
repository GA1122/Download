void TabStrip::StartResizeLayoutTabsFromTouchTimer() {
  constexpr auto kTouchResizeLayoutTime = base::TimeDelta::FromSeconds(2);

  resize_layout_timer_.Stop();
  resize_layout_timer_.Start(FROM_HERE, kTouchResizeLayoutTime, this,
                             &TabStrip::ResizeLayoutTabsFromTouch);
}
