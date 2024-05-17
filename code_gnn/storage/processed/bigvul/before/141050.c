void Document::EnqueueScrollEventForNode(Node* target) {
  Event* scroll_event = target->IsDocumentNode()
                            ? Event::CreateBubble(event_type_names::kScroll)
                            : Event::Create(event_type_names::kScroll);
  scroll_event->SetTarget(target);
  EnsureScriptedAnimationController().EnqueuePerFrameEvent(scroll_event);
}
