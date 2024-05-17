void HTMLMediaElement::cancelPendingEventsAndCallbacks() {
  BLINK_MEDIA_LOG << "cancelPendingEventsAndCallbacks(" << (void*)this << ")";
  m_asyncEventQueue->cancelAllEvents();

  for (HTMLSourceElement* source =
           Traversal<HTMLSourceElement>::firstChild(*this);
       source; source = Traversal<HTMLSourceElement>::nextSibling(*source))
    source->cancelPendingErrorEvent();
}
