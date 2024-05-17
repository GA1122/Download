void Document::updateLayoutTreeForNodeIfNeeded(Node* node)
{
    ASSERT(node);
    if (!node->canParticipateInComposedTree())
        return;
    if (!needsLayoutTreeUpdate())
        return;
    if (!node->inDocument())
        return;

    bool needsRecalc = needsFullLayoutTreeUpdate() || node->needsStyleRecalc() || node->needsStyleInvalidation();

    if (!needsRecalc) {
        for (const ContainerNode* ancestor = LayoutTreeBuilderTraversal::parent(*node); ancestor && !needsRecalc; ancestor = LayoutTreeBuilderTraversal::parent(*ancestor))
            needsRecalc = ancestor->needsStyleRecalc() || ancestor->needsStyleInvalidation() || ancestor->needsAdjacentStyleRecalc();
    }

    if (needsRecalc)
        updateLayoutTreeIfNeeded();
}
