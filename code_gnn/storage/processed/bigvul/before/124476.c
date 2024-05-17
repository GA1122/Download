void RenderBlock::addChildToContinuation(RenderObject* newChild, RenderObject* beforeChild)
{
    RenderBlock* flow = continuationBefore(beforeChild);
    ASSERT(!beforeChild || beforeChild->parent()->isAnonymousColumnSpanBlock() || beforeChild->parent()->isRenderBlock());
    RenderBoxModelObject* beforeChildParent = 0;
    if (beforeChild)
        beforeChildParent = toRenderBoxModelObject(beforeChild->parent());
    else {
        RenderBoxModelObject* cont = flow->continuation();
        if (cont)
            beforeChildParent = cont;
        else
            beforeChildParent = flow;
    }

    if (newChild->isFloatingOrOutOfFlowPositioned()) {
        beforeChildParent->addChildIgnoringContinuation(newChild, beforeChild);
        return;
    }

    bool childIsNormal = newChild->isInline() || !newChild->style()->columnSpan();
    bool bcpIsNormal = beforeChildParent->isInline() || !beforeChildParent->style()->columnSpan();
    bool flowIsNormal = flow->isInline() || !flow->style()->columnSpan();

    if (flow == beforeChildParent) {
        flow->addChildIgnoringContinuation(newChild, beforeChild);
        return;
    }

    if (childIsNormal == bcpIsNormal) {
        beforeChildParent->addChildIgnoringContinuation(newChild, beforeChild);
        return;
    }
    if (flowIsNormal == childIsNormal) {
        flow->addChildIgnoringContinuation(newChild, 0);  
        return;
    }
    beforeChildParent->addChildIgnoringContinuation(newChild, beforeChild);
}
