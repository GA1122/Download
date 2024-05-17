void RenderBlock::adjustForBorderFit(LayoutUnit x, LayoutUnit& left, LayoutUnit& right) const
{
    if (style()->visibility() == VISIBLE) {
        if (childrenInline()) {
            for (RootInlineBox* box = firstRootBox(); box; box = box->nextRootBox()) {
                if (box->firstChild())
                    left = min(left, x + static_cast<LayoutUnit>(box->firstChild()->x()));
                if (box->lastChild())
                    right = max(right, x + static_cast<LayoutUnit>(ceilf(box->lastChild()->logicalRight())));
            }
        } else {
            for (RenderBox* obj = firstChildBox(); obj; obj = obj->nextSiblingBox()) {
                if (!obj->isFloatingOrOutOfFlowPositioned()) {
                    if (obj->isRenderBlockFlow() && !obj->hasOverflowClip())
                        toRenderBlock(obj)->adjustForBorderFit(x + obj->x(), left, right);
                    else if (obj->style()->visibility() == VISIBLE) {
                        left = min(left, x + obj->x());
                        right = max(right, x + obj->x() + obj->width());
                    }
                }
            }
        }
    }
}
