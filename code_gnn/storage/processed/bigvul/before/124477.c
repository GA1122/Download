void RenderBlock::addContinuationWithOutline(RenderInline* flow)
{
    ASSERT(!flow->layer() && !flow->isInlineElementContinuation());

    ContinuationOutlineTableMap* table = continuationOutlineTable();
    ListHashSet<RenderInline*>* continuations = table->get(this);
    if (!continuations) {
        continuations = new ListHashSet<RenderInline*>;
        table->set(this, adoptPtr(continuations));
    }

    continuations->add(flow);
}
