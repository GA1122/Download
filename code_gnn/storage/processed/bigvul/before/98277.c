void FrameLoaderClient::cancelPolicyCheck()
{
    if (m_policyDecision)
        webkit_web_policy_decision_cancel(m_policyDecision);
}
