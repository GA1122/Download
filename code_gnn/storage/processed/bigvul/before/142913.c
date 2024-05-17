void HTMLMediaElement::SizeChanged() {
  BLINK_MEDIA_LOG << "sizeChanged(" << (void*)this << ")";

  DCHECK(HasVideo());   
  if (ready_state_ > kHaveNothing && IsHTMLVideoElement())
    ScheduleEvent(event_type_names::kResize);

  if (GetLayoutObject())
    GetLayoutObject()->UpdateFromElement();
}
