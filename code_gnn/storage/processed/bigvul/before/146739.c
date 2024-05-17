void Document::EnqueueResizeEvent() {
  Event* event = Event::Create(EventTypeNames::resize);
  event->SetTarget(domWindow());
  EnsureScriptedAnimationController().EnqueuePerFrameEvent(event);
}
