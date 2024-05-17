String FrameView::trackedPaintInvalidationRectsAsText() const
{
    TextStream ts;
    if (!m_trackedPaintInvalidationRects.isEmpty()) {
        ts << "(repaint rects\n";
        for (size_t i = 0; i < m_trackedPaintInvalidationRects.size(); ++i)
            ts << "  (rect " << m_trackedPaintInvalidationRects[i].x() << " " << m_trackedPaintInvalidationRects[i].y() << " " << m_trackedPaintInvalidationRects[i].width() << " " << m_trackedPaintInvalidationRects[i].height() << ")\n";
        ts << ")\n";
    }
    return ts.release();
}
