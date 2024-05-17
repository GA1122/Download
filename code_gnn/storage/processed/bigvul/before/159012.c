void PDFiumEngine::ScheduleTouchTimer(const pp::TouchInputEvent& evt) {
  touch_timers_[++next_touch_timer_id_] = evt;
  client_->ScheduleTouchTimerCallback(next_touch_timer_id_,
                                      kTouchLongPressTimeout);
}
