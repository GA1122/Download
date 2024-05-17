void Document::ClearFocusedElementTimerFired(TimerBase*) {
  UpdateStyleAndLayoutTree();

  if (focused_element_ && !focused_element_->IsFocusable())
    focused_element_->blur();
}
