static LayoutPoint ComputeRelativeOffset(const LayoutObject* layout_object,
                                         const ScrollableArea* scroller,
                                         Corner corner) {
  return CornerPointOfRect(RelativeBounds(layout_object, scroller), corner);
}
