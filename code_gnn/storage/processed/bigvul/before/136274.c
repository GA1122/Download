void BlockPainter::PaintChildren(const PaintInfo& paint_info,
                                 const LayoutPoint& paint_offset) {
  for (LayoutBox* child = layout_block_.FirstChildBox(); child;
       child = child->NextSiblingBox())
    PaintChild(*child, paint_info, paint_offset);
}
