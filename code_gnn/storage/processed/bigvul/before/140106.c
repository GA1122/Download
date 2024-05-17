void HTMLMediaElement::setPreload(const AtomicString& preload) {
  BLINK_MEDIA_LOG << "setPreload(" << (void*)this << ", " << preload << ")";
  setAttribute(preloadAttr, preload);
}
