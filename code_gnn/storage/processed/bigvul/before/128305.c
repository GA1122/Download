void FrameView::removeResizerArea(RenderBox& resizerBox)
{
    if (!m_resizerAreas)
        return;

    ResizerAreaSet::iterator it = m_resizerAreas->find(&resizerBox);
    if (it != m_resizerAreas->end())
        m_resizerAreas->remove(it);
}
