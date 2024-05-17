void HTMLMediaElement::scheduleEvent(const AtomicString& eventName) {
  scheduleEvent(Event::createCancelable(eventName));
}
