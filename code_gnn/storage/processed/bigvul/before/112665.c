void DocumentLoader::iconLoadDecisionAvailable()
{
    if (m_frame)
        m_frame->loader()->icon()->loadDecisionReceived(iconDatabase().synchronousLoadDecisionForIconURL(frameLoader()->icon()->url(), this));
}
