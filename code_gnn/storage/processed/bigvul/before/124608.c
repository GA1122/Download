void RenderBlock::paintChild(RenderBox* child, PaintInfo& paintInfo, const LayoutPoint& paintOffset)
{
    LayoutPoint childPoint = flipForWritingModeForChild(child, paintOffset);
    if (!child->hasSelfPaintingLayer() && !child->isFloating())
        child->paint(paintInfo, childPoint);
}
