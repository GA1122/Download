PositionWithAffinity RenderBlock::positionForPoint(const LayoutPoint& point)
{
    if (isTable())
        return RenderBox::positionForPoint(point);

    if (isReplaced()) {
        LayoutUnit pointLogicalLeft = isHorizontalWritingMode() ? point.x() : point.y();
        LayoutUnit pointLogicalTop = isHorizontalWritingMode() ? point.y() : point.x();

        if (pointLogicalLeft < 0)
            return createPositionWithAffinity(caretMinOffset(), DOWNSTREAM);
        if (pointLogicalLeft >= logicalWidth())
            return createPositionWithAffinity(caretMaxOffset(), DOWNSTREAM);
        if (pointLogicalTop < 0)
            return createPositionWithAffinity(caretMinOffset(), DOWNSTREAM);
        if (pointLogicalTop >= logicalHeight())
            return createPositionWithAffinity(caretMaxOffset(), DOWNSTREAM);
    }

    LayoutPoint pointInContents = point;
    offsetForContents(pointInContents);
    LayoutPoint pointInLogicalContents(pointInContents);
    if (!isHorizontalWritingMode())
        pointInLogicalContents = pointInLogicalContents.transposedPoint();

    if (childrenInline())
        return positionForPointWithInlineChildren(pointInLogicalContents);

    RenderBox* lastCandidateBox = lastChildBox();
    while (lastCandidateBox && !isChildHitTestCandidate(lastCandidateBox))
        lastCandidateBox = lastCandidateBox->previousSiblingBox();

    bool blocksAreFlipped = style()->isFlippedBlocksWritingMode();
    if (lastCandidateBox) {
        if (pointInLogicalContents.y() > logicalTopForChild(lastCandidateBox)
            || (!blocksAreFlipped && pointInLogicalContents.y() == logicalTopForChild(lastCandidateBox)))
            return positionForPointRespectingEditingBoundaries(this, lastCandidateBox, pointInContents);

        for (RenderBox* childBox = firstChildBox(); childBox; childBox = childBox->nextSiblingBox()) {
            if (!isChildHitTestCandidate(childBox))
                continue;
            LayoutUnit childLogicalBottom = logicalTopForChild(childBox) + logicalHeightForChild(childBox);
            if (isChildHitTestCandidate(childBox) && (pointInLogicalContents.y() < childLogicalBottom
                || (blocksAreFlipped && pointInLogicalContents.y() == childLogicalBottom)))
                return positionForPointRespectingEditingBoundaries(this, childBox, pointInContents);
        }
    }

    return RenderBox::positionForPoint(point);
}
