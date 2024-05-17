PositionWithAffinity LocalFrame::PositionForPoint(
    const LayoutPoint& frame_point) {
  HitTestResult result = GetEventHandler().HitTestResultAtPoint(frame_point);
  Node* node = result.InnerNodeOrImageMapImage();
  if (!node)
    return PositionWithAffinity();
  LayoutObject* layout_object = node->GetLayoutObject();
  if (!layout_object)
    return PositionWithAffinity();
  const PositionWithAffinity position =
      layout_object->PositionForPoint(result.LocalPoint());
  if (position.IsNull())
    return PositionWithAffinity(FirstPositionInOrBeforeNode(*node));
  return position;
}
