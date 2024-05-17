bool RenderBox::isUnsplittableForPagination() const
{
    return isReplaced() || hasUnsplittableScrollingOverflow() || (parent() && isWritingModeRoot());
}
