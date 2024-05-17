bool Document::hasPendingForcedStyleRecalc() const
{
    return hasPendingVisualUpdate() && !inStyleRecalc() && getStyleChangeType() >= SubtreeStyleChange;
}
