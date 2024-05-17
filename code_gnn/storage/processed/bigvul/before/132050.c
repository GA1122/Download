void LayoutBlockFlow::clipOutFloatingObjects(const LayoutBlock* rootBlock, ClipScope& clipScope,
    const LayoutPoint& rootBlockPhysicalPosition, const LayoutSize& offsetFromRootBlock) const
{
    if (!m_floatingObjects)
        return;

    const FloatingObjectSet& floatingObjectSet = m_floatingObjects->set();
    FloatingObjectSetIterator end = floatingObjectSet.end();
    for (FloatingObjectSetIterator it = floatingObjectSet.begin(); it != end; ++it) {
        const FloatingObject& floatingObject = *it->get();
        LayoutRect floatBox(LayoutPoint(offsetFromRootBlock), floatingObject.layoutObject()->size());
        floatBox.move(positionForFloatIncludingMargin(floatingObject));
        rootBlock->flipForWritingMode(floatBox);
        floatBox.move(rootBlockPhysicalPosition.x(), rootBlockPhysicalPosition.y());

        clipScope.clip(floatBox, SkRegion::kDifference_Op);
    }
}
