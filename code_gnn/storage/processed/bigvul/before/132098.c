void LayoutBlockFlow::markSiblingsWithFloatsForLayout(LayoutBox* floatToRemove)
{
    if (!m_floatingObjects)
        return;

    const FloatingObjectSet& floatingObjectSet = m_floatingObjects->set();
    FloatingObjectSetIterator end = floatingObjectSet.end();

    for (LayoutObject* next = nextSibling(); next; next = next->nextSibling()) {
        if (!next->isLayoutBlockFlow() || (!floatToRemove && (next->isFloatingOrOutOfFlowPositioned() || toLayoutBlockFlow(next)->avoidsFloats())))
            continue;

        LayoutBlockFlow* nextBlock = toLayoutBlockFlow(next);
        for (FloatingObjectSetIterator it = floatingObjectSet.begin(); it != end; ++it) {
            LayoutBox* floatingBox = (*it)->layoutObject();
            if (floatToRemove && floatingBox != floatToRemove)
                continue;
            if (nextBlock->containsFloat(floatingBox))
                nextBlock->markAllDescendantsWithFloatsForLayout(floatingBox);
        }
    }
}
