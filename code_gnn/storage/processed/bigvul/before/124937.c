void RenderBox::repaintDuringLayoutIfMoved(const LayoutRect& oldRect)
{
    if (oldRect.location() != m_frameRect.location()) {
        LayoutRect newRect = m_frameRect;
        m_frameRect = oldRect;
        repaint();
        repaintOverhangingFloats(true);
        m_frameRect = newRect;
        repaint();
        repaintOverhangingFloats(true);
    }
}
