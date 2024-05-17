bool Document::hasPendingForcedStyleRecalc() const
{
    return hasPendingStyleRecalc() && !inStyleRecalc() && styleChangeType() >= SubtreeStyleChange;
}
