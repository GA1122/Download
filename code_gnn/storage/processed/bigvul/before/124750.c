LayoutUnit RenderBlockFlow::nextFloatLogicalBottomBelow(LayoutUnit logicalHeight, ShapeOutsideFloatOffsetMode offsetMode) const
{
    if (!m_floatingObjects)
        return logicalHeight;

    LayoutUnit logicalBottom;
    const FloatingObjectSet& floatingObjectSet = m_floatingObjects->set();
    FloatingObjectSetIterator end = floatingObjectSet.end();
    for (FloatingObjectSetIterator it = floatingObjectSet.begin(); it != end; ++it) {
        FloatingObject* floatingObject = *it;
        LayoutUnit floatLogicalBottom = logicalBottomForFloat(floatingObject);
        ShapeOutsideInfo* shapeOutside = floatingObject->renderer()->shapeOutsideInfo();
        if (shapeOutside && (offsetMode == ShapeOutsideFloatShapeOffset)) {
            LayoutUnit shapeLogicalBottom = logicalTopForFloat(floatingObject) + marginBeforeForChild(floatingObject->renderer()) + shapeOutside->shapeLogicalBottom();
            if (shapeLogicalBottom < floatLogicalBottom)
                floatLogicalBottom = shapeLogicalBottom;
        }
        if (floatLogicalBottom > logicalHeight)
            logicalBottom = logicalBottom ? min(floatLogicalBottom, logicalBottom) : floatLogicalBottom;
    }

    return logicalBottom;
}
