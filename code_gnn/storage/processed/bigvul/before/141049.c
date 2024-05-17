void Document::EnqueueScrollEndEventForNode(Node* target) {
  Event* scroll_end_event =
      target->IsDocumentNode()
          ? Event::CreateBubble(event_type_names::kScrollend)
          : Event::Create(event_type_names::kScrollend);
  scroll_end_event->SetTarget(target);
  EnsureScriptedAnimationController().EnqueuePerFrameEvent(scroll_end_event);
}
