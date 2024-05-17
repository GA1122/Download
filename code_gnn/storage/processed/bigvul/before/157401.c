void HTMLMediaElement::SizeChanged() {
  BLINK_MEDIA_LOG << "sizeChanged(" << (void*)this << ")";

  DCHECK(HasVideo());   
  if (ready_state_ > kHaveNothing && IsHTMLVideoElement())
    ScheduleEvent(EventTypeNames::resize);

  if (GetLayoutObject())
    GetLayoutObject()->UpdateFromElement();
}
