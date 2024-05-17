LayoutUnit LayoutBlockFlow::nextFloatLogicalBottomBelow(LayoutUnit logicalHeight, ShapeOutsideFloatOffsetMode offsetMode) const
{
    if (!m_floatingObjects)
        return logicalHeight;

    LayoutUnit logicalBottom;
    const FloatingObjectSet& floatingObjectSet = m_floatingObjects->set();
    FloatingObjectSetIterator end = floatingObjectSet.end();
    for (FloatingObjectSetIterator it = floatingObjectSet.begin(); it != end; ++it) {
        const FloatingObject& floatingObject = *it->get();
        LayoutUnit floatLogicalBottom = logicalBottomForFloat(floatingObject);
        ShapeOutsideInfo* shapeOutside = floatingObject.layoutObject()->shapeOutsideInfo();
        if (shapeOutside && (offsetMode == ShapeOutsideFloatShapeOffset)) {
            LayoutUnit shapeLogicalBottom = logicalTopForFloat(floatingObject) + marginBeforeForChild(*floatingObject.layoutObject()) + shapeOutside->shapeLogicalBottom();
            if (shapeLogicalBottom < floatLogicalBottom)
                floatLogicalBottom = shapeLogicalBottom;
        }
        if (floatLogicalBottom > logicalHeight)
            logicalBottom = logicalBottom ? std::min(floatLogicalBottom, logicalBottom) : floatLogicalBottom;
    }

    return logicalBottom;
}
