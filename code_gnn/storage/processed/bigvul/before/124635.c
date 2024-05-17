bool RenderBlock::requiresColumns(int desiredColumnCount) const
{
    bool isPaginated = style()->isOverflowPaged() && node() != document().viewportDefiningElement();

    return firstChild()
        && (desiredColumnCount != 1 || !style()->hasAutoColumnWidth() || isPaginated)
        && !firstChild()->isAnonymousColumnsBlock()
        && !firstChild()->isAnonymousColumnSpanBlock();
}
