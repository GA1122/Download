void HTMLMediaElement::sizeChanged() {
  BLINK_MEDIA_LOG << "sizeChanged(" << (void*)this << ")";

  DCHECK(hasVideo());   
  if (m_readyState > kHaveNothing && isHTMLVideoElement())
    scheduleEvent(EventTypeNames::resize);

  if (layoutObject())
    layoutObject()->updateFromElement();
}
