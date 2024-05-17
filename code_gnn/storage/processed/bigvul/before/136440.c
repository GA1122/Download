  void ApplyTransform(const TransformPaintPropertyNode* target_transform) {
    if (target_transform == current_transform_)
      return;
    auto sk_matrix = GetSkMatrix(target_transform);
    if (!sk_matrix.isIdentity())
      cc_list_.push<cc::ConcatOp>(sk_matrix);
  }
