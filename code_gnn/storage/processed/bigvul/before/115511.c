void TestController::didReceiveAuthenticationChallengeInFrame(WKPageRef page, WKFrameRef frame, WKAuthenticationChallengeRef authenticationChallenge)
{
    String message;
    if (!m_handlesAuthenticationChallenges)
        message = "<unknown> - didReceiveAuthenticationChallenge - Simulating cancelled authentication sheet\n";
    else
        message = String::format("<unknown> - didReceiveAuthenticationChallenge - Responding with %s:%s\n", m_authenticationUsername.utf8().data(), m_authenticationPassword.utf8().data());
    m_currentInvocation->outputText(message);

    WKAuthenticationDecisionListenerRef decisionListener = WKAuthenticationChallengeGetDecisionListener(authenticationChallenge);
    if (!m_handlesAuthenticationChallenges) {
        WKAuthenticationDecisionListenerUseCredential(decisionListener, 0);
        return;
    }
    WKRetainPtr<WKStringRef> username(AdoptWK, WKStringCreateWithUTF8CString(m_authenticationUsername.utf8().data()));
    WKRetainPtr<WKStringRef> password(AdoptWK, WKStringCreateWithUTF8CString(m_authenticationPassword.utf8().data()));
    WKRetainPtr<WKCredentialRef> credential(AdoptWK, WKCredentialCreate(username.get(), password.get(), kWKCredentialPersistenceForSession));
    WKAuthenticationDecisionListenerUseCredential(decisionListener, credential.get());
}
