void HTMLMediaElement::SetShouldDelayLoadEvent(bool should_delay) {
  if (should_delay_load_event_ == should_delay)
    return;

  BLINK_MEDIA_LOG << "setShouldDelayLoadEvent(" << (void*)this << ", "
                  << BoolString(should_delay) << ")";

  should_delay_load_event_ = should_delay;
  if (should_delay)
    GetDocument().IncrementLoadEventDelayCount();
  else
    GetDocument().DecrementLoadEventDelayCount();
}
