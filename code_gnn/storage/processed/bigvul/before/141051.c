void Document::EnqueueUniqueAnimationFrameEvent(Event* event) {
  EnsureScriptedAnimationController().EnqueuePerFrameEvent(event);
}
