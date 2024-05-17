void WebPageProxy::canAuthenticateAgainstProtectionSpaceInFrame(uint64_t frameID, const WebCore::ProtectionSpace& coreProtectionSpace, bool& canAuthenticate)
{
    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    RefPtr<WebProtectionSpace> protectionSpace = WebProtectionSpace::create(coreProtectionSpace);
    
    canAuthenticate = m_loaderClient.canAuthenticateAgainstProtectionSpaceInFrame(this, frame, protectionSpace.get());
}
