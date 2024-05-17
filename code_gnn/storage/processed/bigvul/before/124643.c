static bool shouldCheckLines(RenderObject* obj)
{
    return !obj->isFloatingOrOutOfFlowPositioned()
        && obj->isRenderBlock() && obj->style()->height().isAuto()
        && (!obj->isDeprecatedFlexibleBox() || obj->style()->boxOrient() == VERTICAL);
}
