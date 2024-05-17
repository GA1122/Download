void RenderBlock::paintChildren(PaintInfo& paintInfo, const LayoutPoint& paintOffset)
{
    for (RenderBox* child = firstChildBox(); child; child = child->nextSiblingBox())
        paintChild(child, paintInfo, paintOffset);
}
