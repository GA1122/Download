void Document::EnqueueVisualViewportScrollEvent() {
  VisualViewportScrollEvent* event = VisualViewportScrollEvent::Create();
  event->SetTarget(domWindow()->visualViewport());
  EnsureScriptedAnimationController().EnqueuePerFrameEvent(event);
}
