void HTMLMediaElement::setLoop(bool b) {
  BLINK_MEDIA_LOG << "setLoop(" << (void*)this << ", " << boolString(b) << ")";
  setBooleanAttribute(loopAttr, b);
}
