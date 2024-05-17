Node::InsertionNotificationRequest HTMLCanvasElement::InsertedInto(
    ContainerNode& node) {
  SetIsInCanvasSubtree(true);
  return HTMLElement::InsertedInto(node);
}
