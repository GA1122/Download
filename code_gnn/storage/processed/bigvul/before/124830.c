void RenderBox::computeInlineDirectionMargins(RenderBlock* containingBlock, LayoutUnit containerWidth, LayoutUnit childWidth, LayoutUnit& marginStart, LayoutUnit& marginEnd) const
{
    const RenderStyle* containingBlockStyle = containingBlock->style();
    Length marginStartLength = style()->marginStartUsing(containingBlockStyle);
    Length marginEndLength = style()->marginEndUsing(containingBlockStyle);

    if (isFloating() || isInline()) {
        marginStart = minimumValueForLength(marginStartLength, containerWidth);
        marginEnd = minimumValueForLength(marginEndLength, containerWidth);
        return;
    }

    if (containingBlock->isFlexibleBox()) {
        if (marginStartLength.isAuto())
            marginStartLength.setValue(0);
        if (marginEndLength.isAuto())
            marginEndLength.setValue(0);
    }

    LayoutUnit availableWidth = containerWidth;
    if (avoidsFloats() && containingBlock->containsFloats())
        availableWidth = containingBlockAvailableLineWidth();

    if ((marginStartLength.isAuto() && marginEndLength.isAuto() && childWidth < availableWidth)
        || (!marginStartLength.isAuto() && !marginEndLength.isAuto() && containingBlock->style()->textAlign() == WEBKIT_CENTER)) {
        LayoutUnit marginStartWidth = minimumValueForLength(marginStartLength, containerWidth);
        LayoutUnit marginEndWidth = minimumValueForLength(marginEndLength, containerWidth);
        LayoutUnit centeredMarginBoxStart = max<LayoutUnit>(0, (availableWidth - childWidth - marginStartWidth - marginEndWidth) / 2);
        marginStart = centeredMarginBoxStart + marginStartWidth;
        marginEnd = availableWidth - childWidth - marginStart + marginEndWidth;
        return;
    }

    if (marginEndLength.isAuto() && childWidth < availableWidth) {
        marginStart = valueForLength(marginStartLength, containerWidth);
        marginEnd = availableWidth - childWidth - marginStart;
        return;
    }

    bool pushToEndFromTextAlign = !marginEndLength.isAuto() && ((!containingBlockStyle->isLeftToRightDirection() && containingBlockStyle->textAlign() == WEBKIT_LEFT)
        || (containingBlockStyle->isLeftToRightDirection() && containingBlockStyle->textAlign() == WEBKIT_RIGHT));
    if ((marginStartLength.isAuto() && childWidth < availableWidth) || pushToEndFromTextAlign) {
        marginEnd = valueForLength(marginEndLength, containerWidth);
        marginStart = availableWidth - childWidth - marginEnd;
        return;
    }

    marginStart = minimumValueForLength(marginStartLength, containerWidth);
    marginEnd = minimumValueForLength(marginEndLength, containerWidth);
}
