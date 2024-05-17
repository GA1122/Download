void BlockPainter::PaintOverflowControlsIfNeeded(
    const PaintInfo& paint_info,
    const LayoutPoint& paint_offset) {
  if (layout_block_.HasOverflowClip() &&
      layout_block_.Style()->Visibility() == EVisibility::kVisible &&
      ShouldPaintSelfBlockBackground(paint_info.phase)) {
    base::Optional<ClipRecorder> clip_recorder;
    if (!layout_block_.Layer()->IsSelfPaintingLayer()) {
      LayoutRect clip_rect = layout_block_.BorderBoxRect();
      clip_rect.MoveBy(paint_offset);
      clip_recorder.emplace(paint_info.context, layout_block_,
                            DisplayItem::kClipScrollbarsToBoxBounds,
                            PixelSnappedIntRect(clip_rect));
    }
    ScrollableAreaPainter(*layout_block_.Layer()->GetScrollableArea())
        .PaintOverflowControls(paint_info, RoundedIntPoint(paint_offset),
                               false  );
  }
}
