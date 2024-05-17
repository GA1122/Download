void LayerWebKitThread::setNeedsDisplay()
{
    if (m_tiler)
        m_tiler->setNeedsDisplay();
    setNeedsCommit();  
}
