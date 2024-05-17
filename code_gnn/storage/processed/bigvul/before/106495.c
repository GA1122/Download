void WebPageProxy::didReceiveAuthenticationChallenge(uint64_t frameID, const WebCore::AuthenticationChallenge& coreChallenge, uint64_t challengeID)
{
    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    RefPtr<AuthenticationChallengeProxy> authenticationChallenge = AuthenticationChallengeProxy::create(coreChallenge, challengeID, process());
    
    m_loaderClient.didReceiveAuthenticationChallengeInFrame(this, frame, authenticationChallenge.get());
}
