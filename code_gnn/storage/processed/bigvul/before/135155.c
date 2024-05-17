PassRefPtrWillBeRawPtr<Range> Document::caretRangeFromPoint(int x, int y)
{
    if (!layoutView())
        return nullptr;

    HitTestResult result = hitTestInDocument(this, x, y);
    PositionWithAffinity positionWithAffinity = result.position();
    if (positionWithAffinity.position().isNull())
        return nullptr;

    Position rangeCompliantPosition = positionWithAffinity.position().parentAnchoredEquivalent();
    return Range::createAdjustedToTreeScope(*this, rangeCompliantPosition);
}
