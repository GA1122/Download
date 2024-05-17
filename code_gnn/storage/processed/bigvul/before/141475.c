ScrollBehavior PaintLayerScrollableArea::ScrollBehaviorStyle() const {
  return GetLayoutBox()->StyleRef().GetScrollBehavior();
}
