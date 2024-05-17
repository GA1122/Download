void HTMLMediaElement::SetLoop(bool b) {
  BLINK_MEDIA_LOG << "setLoop(" << (void*)this << ", " << BoolString(b) << ")";
  SetBooleanAttribute(kLoopAttr, b);
}
