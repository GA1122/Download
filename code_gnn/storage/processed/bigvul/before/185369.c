 bool LayoutSVGResourceMarker::calculateLocalTransform()
// LayoutSVGContainer::TransformChange LayoutSVGResourceMarker::calculateLocalTransform()
  {
    return selfNeedsLayout();
//     return selfNeedsLayout() ? TransformChange::Full : TransformChange::None;
  }