void HTMLMediaElement::DurationChanged(double duration, bool request_seek) {
  BLINK_MEDIA_LOG << "durationChanged(" << (void*)this << ", " << duration
                  << ", " << BoolString(request_seek) << ")";

  if (duration_ == duration)
    return;

  BLINK_MEDIA_LOG << "durationChanged(" << (void*)this << ") : " << duration_
                  << " -> " << duration;
  duration_ = duration;
  ScheduleEvent(event_type_names::kDurationchange);

  if (GetLayoutObject())
    GetLayoutObject()->UpdateFromElement();

  if (request_seek)
    Seek(duration);
}
