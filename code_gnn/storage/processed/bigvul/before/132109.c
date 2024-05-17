void LayoutBlockFlow::paintSelection(const PaintInfo& paintInfo, const LayoutPoint& paintOffset) const
{
    BlockFlowPainter(*this).paintSelection(paintInfo, paintOffset);
}
