void BlockPainter::PaintInlineBox(const InlineBox& inline_box,
                                  const PaintInfo& paint_info,
                                  const LayoutPoint& paint_offset) {
  if (paint_info.phase != PaintPhase::kForeground &&
      paint_info.phase != PaintPhase::kSelection)
    return;

  DCHECK(paint_info.phase != PaintPhase::kTextClip);

  LayoutPoint child_point = paint_offset;
  if (inline_box.Parent()
          ->GetLineLayoutItem()
          .Style()
          ->IsFlippedBlocksWritingMode()) {
    child_point =
        LineLayoutAPIShim::LayoutObjectFrom(inline_box.GetLineLayoutItem())
            ->ContainingBlock()
            ->FlipForWritingModeForChildForPaint(
                ToLayoutBox(LineLayoutAPIShim::LayoutObjectFrom(
                    inline_box.GetLineLayoutItem())),
                child_point);
  }

  ObjectPainter(
      *LineLayoutAPIShim::ConstLayoutObjectFrom(inline_box.GetLineLayoutItem()))
      .PaintAllPhasesAtomically(paint_info, child_point);
}
