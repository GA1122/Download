void RenderBlockFlow::addOverhangingFloats(RenderBlockFlow* child, bool makeChildPaintOtherFloats)
{
    if (child->hasOverflowClip() || !child->containsFloats() || child->isRenderFlowThread() || child->isRenderRegion() || child->isDocumentElement() || child->hasColumns() || child->isWritingModeRoot())
        return;

    LayoutUnit childLogicalTop = child->logicalTop();
    LayoutUnit childLogicalLeft = child->logicalLeft();

    FloatingObjectSetIterator childEnd = child->m_floatingObjects->set().end();
    for (FloatingObjectSetIterator childIt = child->m_floatingObjects->set().begin(); childIt != childEnd; ++childIt) {
        FloatingObject* floatingObject = *childIt;
        LayoutUnit logicalBottomForFloat = min(this->logicalBottomForFloat(floatingObject), LayoutUnit::max() - childLogicalTop);
        LayoutUnit logicalBottom = childLogicalTop + logicalBottomForFloat;

        if (logicalBottom > logicalHeight()) {
            if (!containsFloat(floatingObject->renderer())) {
                LayoutSize offset = isHorizontalWritingMode() ? LayoutSize(-childLogicalLeft, -childLogicalTop) : LayoutSize(-childLogicalTop, -childLogicalLeft);
                bool shouldPaint = false;

                if (floatingObject->renderer()->enclosingFloatPaintingLayer() == enclosingFloatPaintingLayer()) {
                    floatingObject->setShouldPaint(false);
                    shouldPaint = true;
                }
                if (!m_floatingObjects)
                    createFloatingObjects();

                m_floatingObjects->add(floatingObject->copyToNewContainer(offset, shouldPaint, true));
            }
        } else {
            if (makeChildPaintOtherFloats && !floatingObject->shouldPaint() && !floatingObject->renderer()->hasSelfPaintingLayer()
                && floatingObject->renderer()->isDescendantOf(child) && floatingObject->renderer()->enclosingFloatPaintingLayer() == child->enclosingFloatPaintingLayer()) {
                floatingObject->setShouldPaint(true);
            }

            if (floatingObject->isDescendant())
                child->addOverflowFromChild(floatingObject->renderer(), LayoutSize(xPositionForFloatIncludingMargin(floatingObject), yPositionForFloatIncludingMargin(floatingObject)));
        }
    }
}
