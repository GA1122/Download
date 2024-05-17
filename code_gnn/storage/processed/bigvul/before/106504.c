void WebPageProxy::didRestoreFrameFromPageCache(uint64_t frameID, uint64_t parentFrameID)
{
    MESSAGE_CHECK(m_mainFrame);

    WebFrameProxy* subframe = process()->webFrame(frameID);
    MESSAGE_CHECK(subframe);
    MESSAGE_CHECK(!subframe->parentFrame());
    MESSAGE_CHECK(subframe->page() == m_mainFrame->page());

    WebFrameProxy* parentFrame = process()->webFrame(parentFrameID);
    MESSAGE_CHECK(parentFrame);
    MESSAGE_CHECK(parentFrame->isDescendantOf(m_mainFrame.get()));

    parentFrame->appendChild(subframe);
}
