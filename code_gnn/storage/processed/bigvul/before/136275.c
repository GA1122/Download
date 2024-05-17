void BlockPainter::PaintChildrenOfFlexibleBox(
    const LayoutFlexibleBox& layout_flexible_box,
    const PaintInfo& paint_info,
    const LayoutPoint& paint_offset) {
  for (const LayoutBox* child = layout_flexible_box.GetOrderIterator().First();
       child; child = layout_flexible_box.GetOrderIterator().Next())
    BlockPainter(layout_flexible_box)
        .PaintAllChildPhasesAtomically(*child, paint_info, paint_offset);
}
