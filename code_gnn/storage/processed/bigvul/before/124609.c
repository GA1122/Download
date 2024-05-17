void RenderBlock::paintChildAsInlineBlock(RenderBox* child, PaintInfo& paintInfo, const LayoutPoint& paintOffset)
{
    LayoutPoint childPoint = flipForWritingModeForChild(child, paintOffset);
    if (!child->hasSelfPaintingLayer() && !child->isFloating())
        paintAsInlineBlock(child, paintInfo, childPoint);
}
