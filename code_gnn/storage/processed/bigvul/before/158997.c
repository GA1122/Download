void PDFiumEngine::OnTouchTimerCallback(int id) {
  if (!touch_timers_.count(id))
    return;

  HandleLongPress(touch_timers_[id]);
  KillTouchTimer(id);
}
