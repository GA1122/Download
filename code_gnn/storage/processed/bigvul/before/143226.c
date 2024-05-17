bool Document::needsLayoutTreeUpdateForNode(const Node& node) const
{
    if (!node.canParticipateInFlatTree())
        return false;
    if (!needsLayoutTreeUpdate())
        return false;
    if (!node.inShadowIncludingDocument())
        return false;

    if (needsFullLayoutTreeUpdate() || node.needsStyleRecalc() || node.needsStyleInvalidation())
        return true;
    for (const ContainerNode* ancestor = LayoutTreeBuilderTraversal::parent(node); ancestor; ancestor = LayoutTreeBuilderTraversal::parent(*ancestor)) {
        if (ancestor->needsStyleRecalc() || ancestor->needsStyleInvalidation() || ancestor->needsAdjacentStyleRecalc())
            return true;
    }
    return false;
}
