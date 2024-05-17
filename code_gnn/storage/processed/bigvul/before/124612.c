void RenderBlock::paintColumnRules(PaintInfo& paintInfo, const LayoutPoint& paintOffset)
{
    if (paintInfo.context->paintingDisabled())
        return;

    const Color& ruleColor = resolveColor(CSSPropertyWebkitColumnRuleColor);
    bool ruleTransparent = style()->columnRuleIsTransparent();
    EBorderStyle ruleStyle = style()->columnRuleStyle();
    LayoutUnit ruleThickness = style()->columnRuleWidth();
    LayoutUnit colGap = columnGap();
    bool renderRule = ruleStyle > BHIDDEN && !ruleTransparent;
    if (!renderRule)
        return;

    ColumnInfo* colInfo = columnInfo();
    unsigned colCount = columnCount(colInfo);

    bool antialias = shouldAntialiasLines(paintInfo.context);

    if (colInfo->progressionAxis() == ColumnInfo::InlineAxis) {
        bool leftToRight = style()->isLeftToRightDirection();
        LayoutUnit currLogicalLeftOffset = leftToRight ? LayoutUnit() : contentLogicalWidth();
        LayoutUnit ruleAdd = logicalLeftOffsetForContent();
        LayoutUnit ruleLogicalLeft = leftToRight ? LayoutUnit() : contentLogicalWidth();
        LayoutUnit inlineDirectionSize = colInfo->desiredColumnWidth();
        BoxSide boxSide = isHorizontalWritingMode()
            ? leftToRight ? BSLeft : BSRight
            : leftToRight ? BSTop : BSBottom;

        for (unsigned i = 0; i < colCount; i++) {
            if (leftToRight) {
                ruleLogicalLeft += inlineDirectionSize + colGap / 2;
                currLogicalLeftOffset += inlineDirectionSize + colGap;
            } else {
                ruleLogicalLeft -= (inlineDirectionSize + colGap / 2);
                currLogicalLeftOffset -= (inlineDirectionSize + colGap);
            }

            if (i < colCount - 1) {
                LayoutUnit ruleLeft = isHorizontalWritingMode() ? paintOffset.x() + ruleLogicalLeft - ruleThickness / 2 + ruleAdd : paintOffset.x() + borderLeft() + paddingLeft();
                LayoutUnit ruleRight = isHorizontalWritingMode() ? ruleLeft + ruleThickness : ruleLeft + contentWidth();
                LayoutUnit ruleTop = isHorizontalWritingMode() ? paintOffset.y() + borderTop() + paddingTop() : paintOffset.y() + ruleLogicalLeft - ruleThickness / 2 + ruleAdd;
                LayoutUnit ruleBottom = isHorizontalWritingMode() ? ruleTop + contentHeight() : ruleTop + ruleThickness;
                IntRect pixelSnappedRuleRect = pixelSnappedIntRectFromEdges(ruleLeft, ruleTop, ruleRight, ruleBottom);
                drawLineForBoxSide(paintInfo.context, pixelSnappedRuleRect.x(), pixelSnappedRuleRect.y(), pixelSnappedRuleRect.maxX(), pixelSnappedRuleRect.maxY(), boxSide, ruleColor, ruleStyle, 0, 0, antialias);
            }

            ruleLogicalLeft = currLogicalLeftOffset;
        }
    } else {
        bool topToBottom = !style()->isFlippedBlocksWritingMode();
        LayoutUnit ruleLeft = isHorizontalWritingMode()
            ? borderLeft() + paddingLeft()
            : colGap / 2 - colGap - ruleThickness / 2 + borderBefore() + paddingBefore();
        LayoutUnit ruleWidth = isHorizontalWritingMode() ? contentWidth() : ruleThickness;
        LayoutUnit ruleTop = isHorizontalWritingMode()
            ? colGap / 2 - colGap - ruleThickness / 2 + borderBefore() + paddingBefore()
            : borderStart() + paddingStart();
        LayoutUnit ruleHeight = isHorizontalWritingMode() ? ruleThickness : contentHeight();
        LayoutRect ruleRect(ruleLeft, ruleTop, ruleWidth, ruleHeight);

        if (!topToBottom) {
            if (isHorizontalWritingMode())
                ruleRect.setY(height() - ruleRect.maxY());
            else
                ruleRect.setX(width() - ruleRect.maxX());
        }

        ruleRect.moveBy(paintOffset);

        BoxSide boxSide = isHorizontalWritingMode()
            ? topToBottom ? BSTop : BSBottom
            : topToBottom ? BSLeft : BSRight;

        LayoutSize step(0, topToBottom ? colInfo->columnHeight() + colGap : -(colInfo->columnHeight() + colGap));
        if (!isHorizontalWritingMode())
            step = step.transposedSize();

        for (unsigned i = 1; i < colCount; i++) {
            ruleRect.move(step);
            IntRect pixelSnappedRuleRect = pixelSnappedIntRect(ruleRect);
            drawLineForBoxSide(paintInfo.context, pixelSnappedRuleRect.x(), pixelSnappedRuleRect.y(), pixelSnappedRuleRect.maxX(), pixelSnappedRuleRect.maxY(), boxSide, ruleColor, ruleStyle, 0, 0, antialias);
        }
    }
}
