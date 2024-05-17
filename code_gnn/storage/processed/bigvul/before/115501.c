void TestController::decidePolicyForNavigationAction(WKFramePolicyListenerRef listener)
{
    if (m_policyDelegateEnabled && !m_policyDelegatePermissive) {
        WKFramePolicyListenerIgnore(listener);
        return;
    }

    WKFramePolicyListenerUse(listener);
}
