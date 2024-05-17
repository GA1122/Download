void BlockPainter::PaintChild(const LayoutBox& child,
                              const PaintInfo& paint_info,
                              const LayoutPoint& paint_offset) {
  LayoutPoint child_point =
      layout_block_.FlipForWritingModeForChildForPaint(&child, paint_offset);
  if (!child.HasSelfPaintingLayer() && !child.IsFloating() &&
      !child.IsColumnSpanAll())
    child.Paint(paint_info, child_point);
}
