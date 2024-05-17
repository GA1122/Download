void WebPageProxy::decidePolicyForResponse(uint64_t frameID, const ResourceResponse& response, const ResourceRequest& request, uint64_t listenerID, CoreIPC::ArgumentDecoder* arguments, bool& receivedPolicyAction, uint64_t& policyAction, uint64_t& downloadID)
{
    RefPtr<APIObject> userData;
    WebContextUserMessageDecoder messageDecoder(userData, context());
    if (!arguments->decode(messageDecoder))
        return;

    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    RefPtr<WebFramePolicyListenerProxy> listener = frame->setUpPolicyListenerProxy(listenerID);

    ASSERT(!m_inDecidePolicyForMIMEType);

    m_inDecidePolicyForMIMEType = true;
    m_syncMimeTypePolicyActionIsValid = false;

    if (!m_policyClient.decidePolicyForResponse(this, frame, response, request, listener.get(), userData.get()))
        listener->use();

    m_inDecidePolicyForMIMEType = false;

    receivedPolicyAction = m_syncMimeTypePolicyActionIsValid;
    if (m_syncMimeTypePolicyActionIsValid) {
        policyAction = m_syncMimeTypePolicyAction;
        downloadID = m_syncMimeTypePolicyDownloadID;
    }
}
