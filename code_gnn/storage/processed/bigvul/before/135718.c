IntPoint VisiblePositionToContentsPoint(const VisiblePosition& pos) {
  IntPoint result = AbsoluteSelectionBoundsOf(pos).MinXMaxYCorner();
  result.Move(0, -1);
  return result;
}
