void RenderBox::computeLogicalHeight(LayoutUnit logicalHeight, LayoutUnit logicalTop, LogicalExtentComputedValues& computedValues) const
{
    computedValues.m_extent = logicalHeight;
    computedValues.m_position = logicalTop;

    if (isTableCell() || (isInline() && !isReplaced()))
        return;

    Length h;
    if (isOutOfFlowPositioned())
        computePositionedLogicalHeight(computedValues);
    else {
        RenderBlock* cb = containingBlock();
        bool hasPerpendicularContainingBlock = cb->isHorizontalWritingMode() != isHorizontalWritingMode();

        if (!hasPerpendicularContainingBlock) {
            bool shouldFlipBeforeAfter = cb->style()->writingMode() != style()->writingMode();
            computeBlockDirectionMargins(cb,
                shouldFlipBeforeAfter ? computedValues.m_margins.m_after : computedValues.m_margins.m_before,
                shouldFlipBeforeAfter ? computedValues.m_margins.m_before : computedValues.m_margins.m_after);
        }

        if (isTable()) {
            if (hasPerpendicularContainingBlock) {
                bool shouldFlipBeforeAfter = shouldFlipBeforeAfterMargins(cb->style(), style());
                computeInlineDirectionMargins(cb, containingBlockLogicalWidthForContent(), computedValues.m_extent,
                    shouldFlipBeforeAfter ? computedValues.m_margins.m_after : computedValues.m_margins.m_before,
                    shouldFlipBeforeAfter ? computedValues.m_margins.m_before : computedValues.m_margins.m_after);
            }
            return;
        }

        bool inHorizontalBox = parent()->isDeprecatedFlexibleBox() && parent()->style()->boxOrient() == HORIZONTAL;
        bool stretching = parent()->style()->boxAlign() == BSTRETCH;
        bool treatAsReplaced = shouldComputeSizeAsReplaced() && (!inHorizontalBox || !stretching);
        bool checkMinMaxHeight = false;

        if (hasOverrideHeight() && parent()->isFlexibleBoxIncludingDeprecated())
            h = Length(overrideLogicalContentHeight(), Fixed);
        else if (treatAsReplaced)
            h = Length(computeReplacedLogicalHeight(), Fixed);
        else {
            h = style()->logicalHeight();
            checkMinMaxHeight = true;
        }

        if (h.isAuto() && inHorizontalBox && toRenderDeprecatedFlexibleBox(parent())->isStretchingChildren()) {
            h = Length(parentBox()->contentLogicalHeight() - marginBefore() - marginAfter() - borderAndPaddingLogicalHeight(), Fixed);
            checkMinMaxHeight = false;
        }

        LayoutUnit heightResult;
        if (checkMinMaxHeight) {
            heightResult = computeLogicalHeightUsing(style()->logicalHeight(), computedValues.m_extent - borderAndPaddingLogicalHeight());
            if (heightResult == -1)
                heightResult = computedValues.m_extent;
            heightResult = constrainLogicalHeightByMinMax(heightResult, computedValues.m_extent - borderAndPaddingLogicalHeight());
        } else {
            ASSERT(h.isFixed());
            heightResult = h.value() + borderAndPaddingLogicalHeight();
        }

        computedValues.m_extent = heightResult;

        if (hasPerpendicularContainingBlock) {
            bool shouldFlipBeforeAfter = shouldFlipBeforeAfterMargins(cb->style(), style());
            computeInlineDirectionMargins(cb, containingBlockLogicalWidthForContent(), heightResult,
                shouldFlipBeforeAfter ? computedValues.m_margins.m_after : computedValues.m_margins.m_before,
                shouldFlipBeforeAfter ? computedValues.m_margins.m_before : computedValues.m_margins.m_after);
        }
    }

    bool paginatedContentNeedsBaseHeight = document().printing() && h.isPercent()
        && (isDocumentElement() || (isBody() && document().documentElement()->renderer()->style()->logicalHeight().isPercent())) && !isInline();
    if (stretchesToViewport() || paginatedContentNeedsBaseHeight) {
        LayoutUnit margins = collapsedMarginBefore() + collapsedMarginAfter();
        LayoutUnit visibleHeight = viewLogicalHeightForPercentages();
        if (isDocumentElement())
            computedValues.m_extent = max(computedValues.m_extent, visibleHeight - margins);
        else {
            LayoutUnit marginsBordersPadding = margins + parentBox()->marginBefore() + parentBox()->marginAfter() + parentBox()->borderAndPaddingLogicalHeight();
            computedValues.m_extent = max(computedValues.m_extent, visibleHeight - marginsBordersPadding);
        }
    }
}
