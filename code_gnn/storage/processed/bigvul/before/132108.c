void LayoutBlockFlow::paintFloats(const PaintInfo& paintInfo, const LayoutPoint& paintOffset, bool preservePhase) const
{
    BlockFlowPainter(*this).paintFloats(paintInfo, paintOffset, preservePhase);
}
