void WebPageProxy::didCreateSubframe(uint64_t frameID, uint64_t parentFrameID)
{
    MESSAGE_CHECK(m_mainFrame);

    WebFrameProxy* parentFrame = process()->webFrame(parentFrameID);
    MESSAGE_CHECK(parentFrame);
    MESSAGE_CHECK(parentFrame->isDescendantOf(m_mainFrame.get()));

    MESSAGE_CHECK(process()->canCreateFrame(frameID));
    
    RefPtr<WebFrameProxy> subFrame = WebFrameProxy::create(this, frameID);

    process()->frameCreated(frameID, subFrame.get());

    parentFrame->appendChild(subFrame.get());
}
