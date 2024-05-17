void Document::EnqueueVisualViewportResizeEvent() {
  VisualViewportResizeEvent* event = VisualViewportResizeEvent::Create();
  event->SetTarget(domWindow()->visualViewport());
  EnsureScriptedAnimationController().EnqueuePerFrameEvent(event);
}
