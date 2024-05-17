VisiblePositionInFlatTree VisiblePositionOfHitTestResult(
    const HitTestResult& hit_test_result) {
  return CreateVisiblePosition(FromPositionInDOMTree<EditingInFlatTreeStrategy>(
      hit_test_result.InnerNode()->GetLayoutObject()->PositionForPoint(
          hit_test_result.LocalPoint())));
}
