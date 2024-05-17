void RenderBlock::updateScrollInfoAfterLayout()
{
    if (hasOverflowClip()) {
        if (style()->isFlippedBlocksWritingMode()) {
            layer()->scrollableArea()->updateAfterLayout();
            return;
        }

        if (gDelayUpdateScrollInfo)
            gDelayedUpdateScrollInfoSet->add(this);
        else
            layer()->scrollableArea()->updateAfterLayout();
    }
}
