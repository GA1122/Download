static bool inNormalFlow(LayoutBox* child)
{
    LayoutBlock* curr = child->containingBlock();
    LayoutView* layoutView = child->view();
    while (curr && curr != layoutView) {
        if (curr->isLayoutFlowThread())
            return true;
        if (curr->isFloatingOrOutOfFlowPositioned())
            return false;
        curr = curr->containingBlock();
    }
    return true;
}
