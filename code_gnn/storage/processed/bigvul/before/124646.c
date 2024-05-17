bool RenderBlock::simplifiedLayout()
{
    if (normalChildNeedsLayout() || selfNeedsLayout())
        return false;

    if (!posChildNeedsLayout() && !(needsSimplifiedNormalFlowLayout() || needsPositionedMovementLayout()))
        return false;


    {
        LayoutStateMaintainer statePusher(*this, locationOffset());

        if (needsPositionedMovementLayout() && !tryLayoutDoingPositionedMovementOnly())
            return false;

        FastTextAutosizer::LayoutScope fastTextAutosizerLayoutScope(this);

        if (needsSimplifiedNormalFlowLayout())
            simplifiedNormalFlowLayout();

        if (isRenderFlowThread())
            toRenderFlowThread(this)->applyBreakAfterContent(clientLogicalBottom());

        bool canContainFixedPosObjects = canContainFixedPositionObjects();
        if (posChildNeedsLayout() || needsPositionedMovementLayout() || canContainFixedPosObjects)
            layoutPositionedObjects(false, needsPositionedMovementLayout() ? ForcedLayoutAfterContainingBlockMoved : (!posChildNeedsLayout() && canContainFixedPosObjects ? LayoutOnlyFixedPositionedObjects : DefaultLayout));

        LayoutUnit oldClientAfterEdge = hasRenderOverflow() ? m_overflow->layoutClientAfterEdge() : clientLogicalBottom();
        computeOverflow(oldClientAfterEdge, true);
    }

    updateLayerTransform();

    updateScrollInfoAfterLayout();

    clearNeedsLayout();
    return true;
}
