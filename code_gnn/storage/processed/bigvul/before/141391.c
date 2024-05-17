PaintLayerScrollableArea::ConvertFromContainingEmbeddedContentViewToScrollbar(
    const Scrollbar& scrollbar,
    const IntPoint& parent_point) const {
  LayoutView* view = GetLayoutBox()->View();
  if (!view)
    return parent_point;

  IntPoint point = view->GetFrameView()->ConvertToLayoutObject(*GetLayoutBox(),
                                                               parent_point);

  point.Move(-ScrollbarOffset(scrollbar));
  return point;
}
