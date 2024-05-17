void RenderBlock::paintColumnContents(PaintInfo& paintInfo, const LayoutPoint& paintOffset, bool paintingFloats)
{
    GraphicsContext* context = paintInfo.context;
    ColumnInfo* colInfo = columnInfo();
    unsigned colCount = columnCount(colInfo);
    if (!colCount)
        return;
    LayoutUnit currLogicalTopOffset = 0;
    LayoutUnit colGap = columnGap();
    for (unsigned i = 0; i < colCount; i++) {
        LayoutRect colRect = columnRectAt(colInfo, i);
        flipForWritingMode(colRect);
        LayoutUnit logicalLeftOffset = (isHorizontalWritingMode() ? colRect.x() : colRect.y()) - logicalLeftOffsetForContent();
        LayoutSize offset = isHorizontalWritingMode() ? LayoutSize(logicalLeftOffset, currLogicalTopOffset) : LayoutSize(currLogicalTopOffset, logicalLeftOffset);
        if (colInfo->progressionAxis() == ColumnInfo::BlockAxis) {
            if (isHorizontalWritingMode())
                offset.expand(0, colRect.y() - borderTop() - paddingTop());
            else
                offset.expand(colRect.x() - borderLeft() - paddingLeft(), 0);
        }
        colRect.moveBy(paintOffset);
        PaintInfo info(paintInfo);
        info.rect.intersect(pixelSnappedIntRect(colRect));

        if (!info.rect.isEmpty()) {
            GraphicsContextStateSaver stateSaver(*context);
            LayoutRect clipRect(colRect);

            if (i < colCount - 1) {
                if (isHorizontalWritingMode())
                    clipRect.expand(colGap / 2, 0);
                else
                    clipRect.expand(0, colGap / 2);
            }
            context->clip(pixelSnappedIntRect(clipRect));

            LayoutPoint adjustedPaintOffset = paintOffset + offset;
            if (paintingFloats)
                paintFloats(info, adjustedPaintOffset, paintInfo.phase == PaintPhaseSelection || paintInfo.phase == PaintPhaseTextClip);
            else
                paintContents(info, adjustedPaintOffset);
        }

        LayoutUnit blockDelta = (isHorizontalWritingMode() ? colRect.height() : colRect.width());
        if (style()->isFlippedBlocksWritingMode())
            currLogicalTopOffset += blockDelta;
        else
            currLogicalTopOffset -= blockDelta;
    }
}
