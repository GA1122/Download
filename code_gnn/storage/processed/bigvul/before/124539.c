bool RenderBlock::createsBlockFormattingContext() const
{
    return isInlineBlockOrInlineTable() || isFloatingOrOutOfFlowPositioned() || hasOverflowClip() || (parent() && parent()->isFlexibleBoxIncludingDeprecated())
        || style()->specifiesColumns() || isRenderFlowThread() || isTableCell() || isTableCaption() || isFieldset() || isWritingModeRoot() || isDocumentElement() || style()->columnSpan();
}
