uint64_t WebFrame::setUpPolicyListener(WebCore::FramePolicyFunction policyFunction)
{

    invalidatePolicyListener();

    m_policyListenerID = generateListenerID();
    m_policyFunction = policyFunction;
    return m_policyListenerID;
}
