void DocumentLoader::continueIconLoadWithDecision(IconLoadDecision decision)
{
    ASSERT(m_iconLoadDecisionCallback);
    m_iconLoadDecisionCallback = 0;
    if (m_frame)
        m_frame->loader()->icon()->continueLoadWithDecision(decision);
}
