PaintLayerScrollableArea::ScrollingBackgroundDisplayItemClient::VisualRect()
    const {
  const auto* box = scrollable_area_->GetLayoutBox();
  const auto& paint_offset = box->FirstFragment().PaintOffset();
  auto overflow_clip_rect =
      PixelSnappedIntRect(box->OverflowClipRect(paint_offset));
  auto scroll_size = scrollable_area_->PixelSnappedContentsSize(paint_offset);
  scroll_size = scroll_size.ExpandedTo(overflow_clip_rect.Size());
  LayoutRect result(overflow_clip_rect.Location(), scroll_size);
#if DCHECK_IS_ON()
  if (!RuntimeEnabledFeatures::CompositeAfterPaintEnabled()) {
    DCHECK_EQ(result,
              scrollable_area_->layer_->GraphicsLayerBacking()->VisualRect());
  }
#endif
  return result;
}
