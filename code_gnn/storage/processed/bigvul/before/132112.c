bool LayoutBlockFlow::positionNewFloats(LineWidth* width)
{
    if (!m_floatingObjects)
        return false;

    const FloatingObjectSet& floatingObjectSet = m_floatingObjects->set();
    if (floatingObjectSet.isEmpty())
        return false;

    if (floatingObjectSet.last()->isPlaced())
        return false;

    FloatingObjectSetIterator it = floatingObjectSet.end();
    --it;  
    FloatingObjectSetIterator begin = floatingObjectSet.begin();
    FloatingObject* lastPlacedFloatingObject = nullptr;
    while (it != begin) {
        --it;
        if ((*it)->isPlaced()) {
            lastPlacedFloatingObject = it->get();
            ++it;
            break;
        }
    }

    LayoutUnit logicalTop = logicalHeight();

    if (lastPlacedFloatingObject)
        logicalTop = std::max(logicalTopForFloat(*lastPlacedFloatingObject), logicalTop);

    FloatingObjectSetIterator end = floatingObjectSet.end();
    for (; it != end; ++it) {
        FloatingObject& floatingObject = *it->get();
        if (floatingObject.layoutObject()->containingBlock() != this)
            continue;

        LayoutBox* childBox = floatingObject.layoutObject();

        childBox->setMayNeedPaintInvalidation();

        LayoutUnit childLogicalLeftMargin = style()->isLeftToRightDirection() ? marginStartForChild(*childBox) : marginEndForChild(*childBox);
        if (childBox->style()->clear() & CLEFT)
            logicalTop = std::max(lowestFloatLogicalBottom(FloatingObject::FloatLeft), logicalTop);
        if (childBox->style()->clear() & CRIGHT)
            logicalTop = std::max(lowestFloatLogicalBottom(FloatingObject::FloatRight), logicalTop);

        LayoutPoint floatLogicalLocation = computeLogicalLocationForFloat(floatingObject, logicalTop);

        setLogicalLeftForFloat(floatingObject, floatLogicalLocation.x());

        setLogicalLeftForChild(*childBox, floatLogicalLocation.x() + childLogicalLeftMargin);
        setLogicalTopForChild(*childBox, floatLogicalLocation.y() + marginBeforeForChild(*childBox));

        SubtreeLayoutScope layoutScope(*childBox);
        LayoutState* layoutState = view()->layoutState();
        bool isPaginated = layoutState->isPaginated();
        if (isPaginated && !childBox->needsLayout())
            childBox->markForPaginationRelayoutIfNeeded(layoutScope);

        childBox->layoutIfNeeded();

        if (isPaginated) {
            LayoutUnit newLogicalTop = floatLogicalLocation.y();

            LayoutBlockFlow* childBlockFlow = childBox->isLayoutBlockFlow() ? toLayoutBlockFlow(childBox) : 0;
            if (childBlockFlow && childBlockFlow->paginationStrut()) {
                newLogicalTop += childBlockFlow->paginationStrut();
                childBlockFlow->setPaginationStrut(LayoutUnit());
            } else {
                newLogicalTop = adjustForUnsplittableChild(*childBox, newLogicalTop);
            }

            if (newLogicalTop != floatLogicalLocation.y()) {
                floatingObject.setPaginationStrut(newLogicalTop - floatLogicalLocation.y());

                floatLogicalLocation = computeLogicalLocationForFloat(floatingObject, newLogicalTop);
                setLogicalLeftForFloat(floatingObject, floatLogicalLocation.x());

                setLogicalLeftForChild(*childBox, floatLogicalLocation.x() + childLogicalLeftMargin);
                setLogicalTopForChild(*childBox, floatLogicalLocation.y() + marginBeforeForChild(*childBox));

                if (childBox->isLayoutBlock())
                    childBox->setChildNeedsLayout(MarkOnlyThis);
                childBox->layoutIfNeeded();
            }
        }

        setLogicalTopForFloat(floatingObject, floatLogicalLocation.y());

        setLogicalHeightForFloat(floatingObject, logicalHeightForChild(*childBox) + marginBeforeForChild(*childBox) + marginAfterForChild(*childBox));

        m_floatingObjects->addPlacedObject(floatingObject);

        if (ShapeOutsideInfo* shapeOutside = childBox->shapeOutsideInfo())
            shapeOutside->setReferenceBoxLogicalSize(logicalSizeForChild(*childBox));

        if (width)
            width->shrinkAvailableWidthForNewFloatIfNeeded(floatingObject);
    }
    return true;
}
