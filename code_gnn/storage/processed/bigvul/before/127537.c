void LayerWebKitThread::setNeedsDisplayInRect(const FloatRect& dirtyRect)
{
    if (m_tiler)
        m_tiler->setNeedsDisplay(dirtyRect);
    setNeedsCommit();  
}
