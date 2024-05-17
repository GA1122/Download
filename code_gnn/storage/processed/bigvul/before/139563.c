static LocalFrame* TargetFrame(LocalFrame& frame, Event* event) {
  if (!event)
    return &frame;
  Node* node = event->target()->ToNode();
  if (!node)
    return &frame;
  return node->GetDocument().GetFrame();
}
