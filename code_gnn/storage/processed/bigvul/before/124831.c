static void computeInlineStaticDistance(Length& logicalLeft, Length& logicalRight, const RenderBox* child, const RenderBoxModelObject* containerBlock, LayoutUnit containerLogicalWidth)
{
    if (!logicalLeft.isAuto() || !logicalRight.isAuto())
        return;

    if (child->parent()->style()->direction() == LTR) {
        LayoutUnit staticPosition = child->layer()->staticInlinePosition() - containerBlock->borderLogicalLeft();
        for (RenderObject* curr = child->parent(); curr && curr != containerBlock; curr = curr->container()) {
            if (curr->isBox()) {
                staticPosition += toRenderBox(curr)->logicalLeft();
                if (toRenderBox(curr)->isRelPositioned())
                    staticPosition += toRenderBox(curr)->relativePositionOffset().width();
            } else if (curr->isInline()) {
                if (curr->isRelPositioned()) {
                    if (!curr->style()->logicalLeft().isAuto())
                        staticPosition += curr->style()->logicalLeft().value();
                    else
                        staticPosition -= curr->style()->logicalRight().value();
                }
            }
        }
        logicalLeft.setValue(Fixed, staticPosition);
    } else {
        RenderBox* enclosingBox = child->parent()->enclosingBox();
        LayoutUnit staticPosition = child->layer()->staticInlinePosition() + containerLogicalWidth + containerBlock->borderLogicalLeft();
        for (RenderObject* curr = child->parent(); curr; curr = curr->container()) {
            if (curr->isBox()) {
                if (curr != containerBlock) {
                    staticPosition -= toRenderBox(curr)->logicalLeft();
                    if (toRenderBox(curr)->isRelPositioned())
                        staticPosition -= toRenderBox(curr)->relativePositionOffset().width();
                }
                if (curr == enclosingBox)
                    staticPosition -= enclosingBox->logicalWidth();
            } else if (curr->isInline()) {
                if (curr->isRelPositioned()) {
                    if (!curr->style()->logicalLeft().isAuto())
                        staticPosition -= curr->style()->logicalLeft().value();
                    else
                        staticPosition += curr->style()->logicalRight().value();
                }
            }
            if (curr == containerBlock)
                break;
        }
        logicalRight.setValue(Fixed, staticPosition);
    }
}
