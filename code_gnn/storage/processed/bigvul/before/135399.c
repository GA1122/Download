PassRefPtr<ComputedStyle> Document::styleForPage(int pageIndex)
{
    updateDistribution();
    return ensureStyleResolver().styleForPage(pageIndex);
}
