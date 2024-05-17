void RenderBlock::willBeDestroyed()
{
    m_beingDestroyed = true;

    children()->destroyLeftoverChildren();

    RenderBoxModelObject* continuation = this->continuation();
    if (continuation) {
        continuation->destroy();
        setContinuation(0);
    }

    if (!documentBeingDestroyed()) {
        if (firstLineBox()) {
            if (isSelectionBorder())
                view()->clearSelection();

            if (isAnonymousBlock()) {
                for (InlineFlowBox* box = firstLineBox(); box; box = box->nextLineBox()) {
                    while (InlineBox* childBox = box->firstChild())
                        childBox->remove();
                }
            }
        } else if (parent())
            parent()->dirtyLinesFromChangedChild(this);
    }

    m_lineBoxes.deleteLineBoxes();

    if (UNLIKELY(gDelayedUpdateScrollInfoSet != 0))
        gDelayedUpdateScrollInfoSet->remove(this);

    if (FastTextAutosizer* textAutosizer = document().fastTextAutosizer())
        textAutosizer->destroy(this);

    RenderBox::willBeDestroyed();
}
