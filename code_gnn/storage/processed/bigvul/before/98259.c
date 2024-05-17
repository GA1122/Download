String WebFrame::layerTreeAsText() const
{
    if (!m_coreFrame)
        return "";

    return m_coreFrame->layerTreeAsText();
}
