static inline bool isChildHitTestCandidate(RenderBox* box)
{
    return box->height() && box->style()->visibility() == VISIBLE && !box->isFloatingOrOutOfFlowPositioned();
}
