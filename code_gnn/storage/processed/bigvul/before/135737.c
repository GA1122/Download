void DispatchCompositionUpdateEvent(LocalFrame& frame, const String& text) {
  Element* target = frame.GetDocument()->FocusedElement();
  if (!target)
    return;

  CompositionEvent* event = CompositionEvent::Create(
      EventTypeNames::compositionupdate, frame.DomWindow(), text);
  target->DispatchEvent(event);
}
