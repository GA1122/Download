static PositionWithAffinity positionForPointRespectingEditingBoundaries(RenderBlock* parent, RenderBox* child, const LayoutPoint& pointInParentCoordinates)
{
    LayoutPoint childLocation = child->location();
    if (child->isInFlowPositioned())
        childLocation += child->offsetForInFlowPosition();

    LayoutPoint pointInChildCoordinates(toLayoutPoint(pointInParentCoordinates - childLocation));

    Node* childNode = child->nonPseudoNode();
    if (!childNode)
        return child->positionForPoint(pointInChildCoordinates);

    RenderObject* ancestor = parent;
    while (ancestor && !ancestor->nonPseudoNode())
        ancestor = ancestor->parent();

    if (isEditingBoundary(ancestor, child))
        return child->positionForPoint(pointInChildCoordinates);

    LayoutUnit childMiddle = parent->logicalWidthForChild(child) / 2;
    LayoutUnit logicalLeft = parent->isHorizontalWritingMode() ? pointInChildCoordinates.x() : pointInChildCoordinates.y();
    if (logicalLeft < childMiddle)
        return ancestor->createPositionWithAffinity(childNode->nodeIndex(), DOWNSTREAM);
    return ancestor->createPositionWithAffinity(childNode->nodeIndex() + 1, UPSTREAM);
}
