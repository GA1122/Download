void RenderFlexibleBox::layoutBlock(bool relayoutChildren)
{
    ASSERT(needsLayout());

    if (!relayoutChildren && simplifiedLayout())
        return;

    LayoutRepainter repainter(*this, checkForRepaintDuringLayout());

    if (updateLogicalWidthAndColumnWidth())
        relayoutChildren = true;

    LayoutUnit previousHeight = logicalHeight();
    setLogicalHeight(borderAndPaddingLogicalHeight() + scrollbarLogicalHeight());

    {
        LayoutStateMaintainer statePusher(*this, locationOffset());

        m_numberOfInFlowChildrenOnFirstLine = -1;

        RenderBlock::startDelayUpdateScrollInfo();

        prepareOrderIteratorAndMargins();

        ChildFrameRects oldChildRects;
        appendChildFrameRects(oldChildRects);

        layoutFlexItems(relayoutChildren);

        RenderBlock::finishDelayUpdateScrollInfo();

        if (logicalHeight() != previousHeight)
            relayoutChildren = true;

        layoutPositionedObjects(relayoutChildren || isDocumentElement());

        computeRegionRangeForBlock(flowThreadContainingBlock());

        repaintChildrenDuringLayoutIfMoved(oldChildRects);
        computeOverflow(clientLogicalBottomAfterRepositioning());
    }

    updateLayerTransform();

    updateScrollInfoAfterLayout();

    repainter.repaintAfterLayout();

    clearNeedsLayout();
}
