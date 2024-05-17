LayoutUnit RenderBox::computeReplacedLogicalWidthUsing(Length logicalWidth) const
{
    switch (logicalWidth.type()) {
        case Fixed:
            return adjustContentBoxLogicalWidthForBoxSizing(logicalWidth.value());
        case MinContent:
        case MaxContent: {
            LayoutUnit availableLogicalWidth = 0;
            return computeIntrinsicLogicalWidthUsing(logicalWidth, availableLogicalWidth, borderAndPaddingLogicalWidth()) - borderAndPaddingLogicalWidth();
        }
        case FitContent:
        case FillAvailable:
        case Percent:
        case Calculated: {
            const LayoutUnit cw = isOutOfFlowPositioned() ? containingBlockLogicalWidthForPositioned(toRenderBoxModelObject(container())) : containingBlockLogicalWidthForContent();
            Length containerLogicalWidth = containingBlock()->style()->logicalWidth();
            if (logicalWidth.isIntrinsic())
                return computeIntrinsicLogicalWidthUsing(logicalWidth, cw, borderAndPaddingLogicalWidth()) - borderAndPaddingLogicalWidth();
            if (cw > 0 || (!cw && (containerLogicalWidth.isFixed() || containerLogicalWidth.isPercent())))
                return adjustContentBoxLogicalWidthForBoxSizing(minimumValueForLength(logicalWidth, cw));
        }
        case Intrinsic:
        case MinIntrinsic:
        case Auto:
        case Undefined:
            return intrinsicLogicalWidth();
        case ExtendToZoom:
        case DeviceWidth:
        case DeviceHeight:
            break;
    }

    ASSERT_NOT_REACHED();
    return 0;
}
