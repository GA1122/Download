static bool inNormalFlow(RenderBox* child)
{
    RenderBlock* curr = child->containingBlock();
    RenderView* renderView = child->view();
    while (curr && curr != renderView) {
        if (curr->hasColumns() || curr->isRenderFlowThread())
            return true;
        if (curr->isFloatingOrOutOfFlowPositioned())
            return false;
        curr = curr->containingBlock();
    }
    return true;
}
