void ConversionContext::SwitchToTransform(
    const TransformPaintPropertyNode* target_transform) {
  if (target_transform == current_transform_)
    return;

  EndTransform();
  if (target_transform == current_transform_)
    return;

  auto sk_matrix = GetSkMatrix(target_transform);
  if (sk_matrix.isIdentity())
    return;

  cc_list_.StartPaint();
  cc_list_.push<cc::SaveOp>();
  cc_list_.push<cc::ConcatOp>(sk_matrix);
  cc_list_.EndPaintOfPairedBegin();
  previous_transform_ = current_transform_;
  current_transform_ = target_transform;
}
