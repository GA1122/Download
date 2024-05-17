void HTMLMediaElement::setVolume(double vol, ExceptionState& exception_state) {
  BLINK_MEDIA_LOG << "setVolume(" << (void*)this << ", " << vol << ")";

  if (volume_ == vol)
    return;

  if (vol < 0.0f || vol > 1.0f) {
    exception_state.ThrowDOMException(
        kIndexSizeError,
        ExceptionMessages::IndexOutsideRange(
            "volume", vol, 0.0, ExceptionMessages::kInclusiveBound, 1.0,
            ExceptionMessages::kInclusiveBound));
    return;
  }

  volume_ = vol;

  if (GetWebMediaPlayer())
    GetWebMediaPlayer()->SetVolume(EffectiveMediaVolume());
  ScheduleEvent(EventTypeNames::volumechange);
}