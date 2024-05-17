static bool canHaveOverflowScrollbars(const RenderBox& box)
{
    return !box.isRenderView() && box.document().viewportDefiningElement() != box.node();
}
