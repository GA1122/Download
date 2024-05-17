void RenderBlockFlow::adjustForBorderFit(LayoutUnit x, LayoutUnit& left, LayoutUnit& right) const
{
    RenderBlock::adjustForBorderFit(x, left, right);
    if (m_floatingObjects && style()->visibility() == VISIBLE) {
        const FloatingObjectSet& floatingObjectSet = m_floatingObjects->set();
        FloatingObjectSetIterator end = floatingObjectSet.end();
        for (FloatingObjectSetIterator it = floatingObjectSet.begin(); it != end; ++it) {
            FloatingObject* floatingObject = *it;
            if (floatingObject->shouldPaint()) {
                LayoutUnit floatLeft = xPositionForFloatIncludingMargin(floatingObject) - floatingObject->renderer()->x();
                LayoutUnit floatRight = floatLeft + floatingObject->renderer()->width();
                left = min(left, floatLeft);
                right = max(right, floatRight);
            }
        }
    }
}
