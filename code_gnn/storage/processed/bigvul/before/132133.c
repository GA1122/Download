void LayoutBlockFlow::styleDidChange(StyleDifference diff, const ComputedStyle* oldStyle)
{
    LayoutBlock::styleDidChange(diff, oldStyle);

    bool canPropagateFloatIntoSibling = !isFloatingOrOutOfFlowPositioned() && !avoidsFloats();
    if (diff.needsFullLayout() && s_canPropagateFloatIntoSibling && !canPropagateFloatIntoSibling && hasOverhangingFloats()) {
        LayoutBlockFlow* parentBlockFlow = this;
        const FloatingObjectSet& floatingObjectSet = m_floatingObjects->set();
        FloatingObjectSetIterator end = floatingObjectSet.end();

        for (LayoutObject* curr = parent(); curr && !curr->isLayoutView(); curr = curr->parent()) {
            if (curr->isLayoutBlockFlow()) {
                LayoutBlockFlow* currBlock = toLayoutBlockFlow(curr);

                if (currBlock->hasOverhangingFloats()) {
                    for (FloatingObjectSetIterator it = floatingObjectSet.begin(); it != end; ++it) {
                        LayoutBox* layoutBox = (*it)->layoutObject();
                        if (currBlock->hasOverhangingFloat(layoutBox)) {
                            parentBlockFlow = currBlock;
                            break;
                        }
                    }
                }
            }
        }

        parentBlockFlow->markAllDescendantsWithFloatsForLayout();
        parentBlockFlow->markSiblingsWithFloatsForLayout();
    }

    if (diff.needsFullLayout() || !oldStyle)
        createOrDestroyMultiColumnFlowThreadIfNeeded(oldStyle);
    if (oldStyle) {
        if (LayoutMultiColumnFlowThread* flowThread = multiColumnFlowThread()) {
            if (!style()->columnRuleEquivalent(oldStyle)) {
                flowThread->columnRuleStyleDidChange();
            }
        }
    }
}
