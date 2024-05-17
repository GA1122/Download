void ConversionContext::StartClip(
    const FloatRoundedRect& combined_clip_rect,
    const ClipPaintPropertyNode* lowest_combined_clip_node) {
  if (lowest_combined_clip_node->LocalTransformSpace() != current_transform_)
    EndTransform();
  cc_list_.StartPaint();
  cc_list_.push<cc::SaveOp>();
  ApplyTransform(lowest_combined_clip_node->LocalTransformSpace());
  const bool antialias = true;
  if (combined_clip_rect.IsRounded()) {
    cc_list_.push<cc::ClipRRectOp>(combined_clip_rect, SkClipOp::kIntersect,
                                   antialias);
  } else {
    cc_list_.push<cc::ClipRectOp>(combined_clip_rect.Rect(),
                                  SkClipOp::kIntersect, antialias);
  }
  if (lowest_combined_clip_node->ClipPath()) {
    cc_list_.push<cc::ClipPathOp>(
        lowest_combined_clip_node->ClipPath()->GetSkPath(),
        SkClipOp::kIntersect, antialias);
  }
  cc_list_.EndPaintOfPairedBegin();

  PushState(StateEntry::kClip, 1);
  current_clip_ = lowest_combined_clip_node;
  current_transform_ = lowest_combined_clip_node->LocalTransformSpace();
}
