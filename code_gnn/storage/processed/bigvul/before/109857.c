void Document::updateDistributionIfNeeded()
{
    if (!childNeedsDistributionRecalc())
        return;
    TRACE_EVENT0("webkit", "Document::recalcDistribution");
    recalcDistribution();
}
