LayoutSize RenderBox::offsetFromContainer(RenderObject* o, const LayoutPoint& point, bool* offsetDependsOnPoint) const
{
    ASSERT(o == container());

    LayoutSize offset;
    if (isInFlowPositioned())
        offset += offsetForInFlowPosition();

    if (!isInline() || isReplaced()) {
        if (!style()->hasOutOfFlowPosition() && o->hasColumns()) {
            RenderBlock* block = toRenderBlock(o);
            LayoutRect columnRect(frameRect());
            block->adjustStartEdgeForWritingModeIncludingColumns(columnRect);
            offset += toSize(columnRect.location());
            LayoutPoint columnPoint = block->flipForWritingModeIncludingColumns(point + offset);
            offset = toLayoutSize(block->flipForWritingModeIncludingColumns(toLayoutPoint(offset)));
            o->adjustForColumns(offset, columnPoint);
            offset = block->flipForWritingMode(offset);

            if (offsetDependsOnPoint)
                *offsetDependsOnPoint = true;
        } else
            offset += topLeftLocationOffset();
    }

    if (o->hasOverflowClip())
        offset -= toRenderBox(o)->scrolledContentOffset();

    if (style()->position() == AbsolutePosition && o->isInFlowPositioned() && o->isRenderInline())
        offset += toRenderInline(o)->offsetForInFlowPositionedInline(*this);

    if (offsetDependsOnPoint)
        *offsetDependsOnPoint |= o->isRenderFlowThread();

    return offset;
}
