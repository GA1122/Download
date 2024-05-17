DispatchEventResult dispatchTextInputEvent(LocalFrame* frame,
                                           const String& text) {
  if (Element* target = frame->document()->focusedElement()) {
    TextEvent* event = TextEvent::create(frame->domWindow(), text,
                                         TextEventInputIncrementalInsertion);
    event->setUnderlyingEvent(nullptr);
    return target->dispatchEvent(event);
  }
  return DispatchEventResult::CanceledBeforeDispatch;
}
