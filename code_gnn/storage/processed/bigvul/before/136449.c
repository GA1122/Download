  SkMatrix GetSkMatrix(
      const TransformPaintPropertyNode* target_transform) const {
    return SkMatrix(TransformationMatrix::ToSkMatrix44(
        GeometryMapper::SourceToDestinationProjection(target_transform,
                                                      current_transform_)));
  }
