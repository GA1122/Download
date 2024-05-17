void BlockPainter::PaintObject(const PaintInfo& paint_info,
                               const LayoutPoint& paint_offset) {
  if (layout_block_.IsTruncated())
    return;

  const PaintPhase paint_phase = paint_info.phase;

  if (ShouldPaintSelfBlockBackground(paint_phase)) {
    if (layout_block_.Style()->Visibility() == EVisibility::kVisible &&
        layout_block_.HasBoxDecorationBackground())
      layout_block_.PaintBoxDecorationBackground(paint_info, paint_offset);
    if (RuntimeEnabledFeatures::SlimmingPaintV2Enabled())
      PaintScrollHitTestDisplayItem(paint_info);
    if (paint_phase == PaintPhase::kSelfBlockBackgroundOnly)
      return;
  }

  if (paint_phase == PaintPhase::kMask &&
      layout_block_.Style()->Visibility() == EVisibility::kVisible) {
    layout_block_.PaintMask(paint_info, paint_offset);
    return;
  }

  if (paint_phase == PaintPhase::kClippingMask &&
      layout_block_.Style()->Visibility() == EVisibility::kVisible) {
    DCHECK(!RuntimeEnabledFeatures::SlimmingPaintV175Enabled());
    BoxPainter(layout_block_).PaintClippingMask(paint_info, paint_offset);
    return;
  }

  if (paint_phase == PaintPhase::kForeground && paint_info.IsPrinting())
    ObjectPainter(layout_block_)
        .AddPDFURLRectIfNeeded(paint_info, paint_offset);

  if (paint_phase != PaintPhase::kSelfOutlineOnly) {
    base::Optional<ScopedPaintChunkProperties> scoped_scroll_property;
    base::Optional<ScrollRecorder> scroll_recorder;
    base::Optional<PaintInfo> scrolled_paint_info;
    if (RuntimeEnabledFeatures::SlimmingPaintV175Enabled()) {
      if (const auto* fragment = paint_info.FragmentToPaint(layout_block_)) {
        const auto* object_properties = fragment->PaintProperties();
        auto* scroll_translation = object_properties
                                       ? object_properties->ScrollTranslation()
                                       : nullptr;
        if (scroll_translation) {
          scoped_scroll_property.emplace(
              paint_info.context.GetPaintController(), scroll_translation,
              layout_block_, DisplayItem::PaintPhaseToScrollType(paint_phase));
          scrolled_paint_info.emplace(paint_info);
          if (RuntimeEnabledFeatures::SlimmingPaintV2Enabled()) {
            scrolled_paint_info->UpdateCullRectForScrollingContents(
                EnclosingIntRect(layout_block_.OverflowClipRect(paint_offset)),
                scroll_translation->Matrix().ToAffineTransform());
          } else {
            scrolled_paint_info->UpdateCullRect(
                scroll_translation->Matrix().ToAffineTransform());
          }
        }
      }
    } else if (layout_block_.HasOverflowClip()) {
      IntSize scroll_offset = layout_block_.ScrolledContentOffset();
      if (layout_block_.Layer()->ScrollsOverflow() || !scroll_offset.IsZero()) {
        scroll_recorder.emplace(paint_info.context, layout_block_, paint_phase,
                                scroll_offset);
        scrolled_paint_info.emplace(paint_info);
        AffineTransform transform;
        transform.Translate(-scroll_offset.Width(), -scroll_offset.Height());
        scrolled_paint_info->UpdateCullRect(transform);
      }
    }

    const PaintInfo& contents_paint_info =
        scrolled_paint_info ? *scrolled_paint_info : paint_info;

    if (layout_block_.IsLayoutBlockFlow()) {
      BlockFlowPainter block_flow_painter(ToLayoutBlockFlow(layout_block_));
      block_flow_painter.PaintContents(contents_paint_info, paint_offset);
      if (paint_phase == PaintPhase::kFloat ||
          paint_phase == PaintPhase::kSelection ||
          paint_phase == PaintPhase::kTextClip)
        block_flow_painter.PaintFloats(contents_paint_info, paint_offset);
    } else {
      PaintContents(contents_paint_info, paint_offset);
    }
  }

  if (ShouldPaintSelfOutline(paint_phase))
    ObjectPainter(layout_block_).PaintOutline(paint_info, paint_offset);

  if (paint_phase == PaintPhase::kForeground &&
      layout_block_.ShouldPaintCarets())
    PaintCarets(paint_info, paint_offset);
}
