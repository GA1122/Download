int GetLeftPaddingForBulletedItems(bool parent_bulleted) {
  return LayoutDelegate::Get()->GetMetric(
             LayoutDelegate::Metric::RELATED_CONTROL_HORIZONTAL_SPACING) *
          (parent_bulleted ? 2 : 1);
 }
