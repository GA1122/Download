void WebPagePrivate::setTextZoomFactor(float textZoomFactor)
{
    if (!m_mainFrame)
        return;

    m_mainFrame->setTextZoomFactor(textZoomFactor);
}
