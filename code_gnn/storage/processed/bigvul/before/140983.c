void Document::ClearFocusedElementSoon() {
  if (!clear_focused_element_timer_.IsActive())
    clear_focused_element_timer_.StartOneShot(TimeDelta(), FROM_HERE);
}
