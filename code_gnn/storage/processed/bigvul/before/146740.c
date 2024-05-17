void Document::EnqueueScrollEventForNode(Node* target) {
  Event* scroll_event = target->IsDocumentNode()
                            ? Event::CreateBubble(EventTypeNames::scroll)
                            : Event::Create(EventTypeNames::scroll);
  scroll_event->SetTarget(target);
  EnsureScriptedAnimationController().EnqueuePerFrameEvent(scroll_event);
}
