void RenderBlock::collapseAnonymousBlockChild(RenderBlock* parent, RenderBlock* child)
{
    if (child->beingDestroyed())
        return;
    parent->setNeedsLayoutAndPrefWidthsRecalc();
    parent->setChildrenInline(child->childrenInline());
    RenderObject* nextSibling = child->nextSibling();

    RenderFlowThread* childFlowThread = child->flowThreadContainingBlock();
    CurrentRenderFlowThreadMaintainer flowThreadMaintainer(childFlowThread);

    parent->children()->removeChildNode(parent, child, child->hasLayer());
    child->moveAllChildrenTo(parent, nextSibling, child->hasLayer());
    child->deleteLineBoxTree();
    child->destroy();
}
