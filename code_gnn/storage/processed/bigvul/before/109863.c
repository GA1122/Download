void Document::updateStyleForNodeIfNeeded(Node* node)
{
    if (!hasPendingForcedStyleRecalc() && !childNeedsStyleRecalc() && !needsStyleRecalc())
        return;

    bool needsStyleRecalc = hasPendingForcedStyleRecalc();
    for (Node* ancestor = node; ancestor && !needsStyleRecalc; ancestor = ancestor->parentOrShadowHostNode())
        needsStyleRecalc = ancestor->needsStyleRecalc();
    if (needsStyleRecalc)
        updateStyleIfNeeded();
}
