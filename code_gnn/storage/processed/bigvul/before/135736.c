void DispatchCompositionEndEvent(LocalFrame& frame, const String& text) {
  Element* target = frame.GetDocument()->FocusedElement();
  if (!target)
    return;

  CompositionEvent* event = CompositionEvent::Create(
      EventTypeNames::compositionend, frame.DomWindow(), text);
  target->DispatchEvent(event);
}
