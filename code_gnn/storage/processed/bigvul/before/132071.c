bool LayoutBlockFlow::hitTestFloats(HitTestResult& result, const HitTestLocation& locationInContainer, const LayoutPoint& accumulatedOffset)
{
    if (!m_floatingObjects)
        return false;

    LayoutPoint adjustedLocation = accumulatedOffset;
    if (isLayoutView()) {
        DoublePoint position = toLayoutView(this)->frameView()->scrollPositionDouble();
        adjustedLocation.move(position.x(), position.y());
    }

    const FloatingObjectSet& floatingObjectSet = m_floatingObjects->set();
    FloatingObjectSetIterator begin = floatingObjectSet.begin();
    for (FloatingObjectSetIterator it = floatingObjectSet.end(); it != begin;) {
        --it;
        const FloatingObject& floatingObject = *it->get();
        if (floatingObject.isDirectlyContained() && !floatingObject.layoutObject()->hasSelfPaintingLayer()) {
            LayoutUnit xOffset = xPositionForFloatIncludingMargin(floatingObject) - floatingObject.layoutObject()->location().x();
            LayoutUnit yOffset = yPositionForFloatIncludingMargin(floatingObject) - floatingObject.layoutObject()->location().y();
            LayoutPoint childPoint = flipFloatForWritingModeForChild(floatingObject, adjustedLocation + LayoutSize(xOffset, yOffset));
            if (floatingObject.layoutObject()->hitTest(result, locationInContainer, childPoint)) {
                updateHitTestResult(result, locationInContainer.point() - toLayoutSize(childPoint));
                return true;
            }
        }
    }

    return false;
}
