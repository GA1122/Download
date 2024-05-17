RenderBlockFlow* RenderBlock::columnsBlockForSpanningElement(RenderObject* newChild)
{
    RenderBlockFlow* columnsBlockAncestor = 0;
    if (!newChild->isText() && newChild->style()->columnSpan() && !newChild->isBeforeOrAfterContent()
        && !newChild->isFloatingOrOutOfFlowPositioned() && !newChild->isInline() && !isAnonymousColumnSpanBlock()) {
        columnsBlockAncestor = containingColumnsBlock(false);
        if (columnsBlockAncestor) {
            RenderObject* curr = this;
            while (curr && curr != columnsBlockAncestor) {
                if (curr->isRenderBlock() && toRenderBlock(curr)->continuation()) {
                    columnsBlockAncestor = 0;
                    break;
                }
                curr = curr->parent();
            }
        }
    }
    return columnsBlockAncestor;
}
