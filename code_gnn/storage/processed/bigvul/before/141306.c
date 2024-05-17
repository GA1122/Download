Range* Document::caretRangeFromPoint(int x, int y) {
  if (!GetLayoutView())
    return nullptr;

  HitTestResult result = HitTestInDocument(this, x, y);
  PositionWithAffinity position_with_affinity = result.GetPosition();
  if (position_with_affinity.IsNull())
    return nullptr;

  Position range_compliant_position =
      position_with_affinity.GetPosition().ParentAnchoredEquivalent();
  return CreateRangeAdjustedToTreeScope(*this, range_compliant_position);
}
