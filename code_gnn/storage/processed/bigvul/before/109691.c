bool Document::hasPendingForcedStyleRecalc() const
{
    return hasPendingStyleRecalc() && styleChangeType() >= SubtreeStyleChange;
}
