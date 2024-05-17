void FrameView::addResizerArea(RenderBox& resizerBox)
{
    if (!m_resizerAreas)
        m_resizerAreas = adoptPtr(new ResizerAreaSet);
    m_resizerAreas->add(&resizerBox);
}
