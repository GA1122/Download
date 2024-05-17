void WebPageProxy::decidePolicyForNavigationAction(uint64_t frameID, uint32_t opaqueNavigationType, uint32_t opaqueModifiers, int32_t opaqueMouseButton, const ResourceRequest& request, uint64_t listenerID, CoreIPC::ArgumentDecoder* arguments, bool& receivedPolicyAction, uint64_t& policyAction, uint64_t& downloadID)
{
    RefPtr<APIObject> userData;
    WebContextUserMessageDecoder messageDecoder(userData, context());
    if (!arguments->decode(messageDecoder))
        return;

    if (request.url() != pendingAPIRequestURL())
        clearPendingAPIRequestURL();

    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    NavigationType navigationType = static_cast<NavigationType>(opaqueNavigationType);
    WebEvent::Modifiers modifiers = static_cast<WebEvent::Modifiers>(opaqueModifiers);
    WebMouseEvent::Button mouseButton = static_cast<WebMouseEvent::Button>(opaqueMouseButton);
    
    RefPtr<WebFramePolicyListenerProxy> listener = frame->setUpPolicyListenerProxy(listenerID);

    ASSERT(!m_inDecidePolicyForNavigationAction);

    m_inDecidePolicyForNavigationAction = true;
    m_syncNavigationActionPolicyActionIsValid = false;
    
    if (!m_policyClient.decidePolicyForNavigationAction(this, frame, navigationType, modifiers, mouseButton, request, listener.get(), userData.get()))
        listener->use();

    m_inDecidePolicyForNavigationAction = false;

    receivedPolicyAction = m_syncNavigationActionPolicyActionIsValid;
    if (m_syncNavigationActionPolicyActionIsValid) {
        policyAction = m_syncNavigationActionPolicyAction;
        downloadID = m_syncNavigationActionPolicyDownloadID;
    }
}
