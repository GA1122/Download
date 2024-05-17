void WebPagePrivate::authenticationChallenge(const KURL& url, const ProtectionSpace& protectionSpace, const Credential& inputCredential)
{
    AuthenticationChallengeManager* authmgr = AuthenticationChallengeManager::instance();
    BlackBerry::Platform::String username;
    BlackBerry::Platform::String password;

#if !defined(PUBLIC_BUILD) || !PUBLIC_BUILD
    if (m_dumpRenderTree) {
        Credential credential(inputCredential, inputCredential.persistence());
        if (m_dumpRenderTree->didReceiveAuthenticationChallenge(credential))
            authmgr->notifyChallengeResult(url, protectionSpace, AuthenticationChallengeSuccess, credential);
        else
            authmgr->notifyChallengeResult(url, protectionSpace, AuthenticationChallengeCancelled, inputCredential);
        return;
    }
#endif

#if ENABLE(BLACKBERRY_CREDENTIAL_PERSIST)
    if (m_webSettings->isCredentialAutofillEnabled() && !m_webSettings->isPrivateBrowsingEnabled())
        credentialManager().autofillAuthenticationChallenge(protectionSpace, username, password);
#endif

    bool isConfirmed = m_client->authenticationChallenge(protectionSpace.realm().characters(), protectionSpace.realm().length(), username, password);

#if ENABLE(BLACKBERRY_CREDENTIAL_PERSIST)
    Credential credential(username, password, CredentialPersistencePermanent);
    if (m_webSettings->isCredentialAutofillEnabled() && !m_webSettings->isPrivateBrowsingEnabled() && isConfirmed)
        credentialManager().saveCredentialIfConfirmed(this, CredentialTransformData(url, protectionSpace, credential));
#else
    Credential credential(username, password, CredentialPersistenceNone);
#endif

    if (isConfirmed)
        authmgr->notifyChallengeResult(url, protectionSpace, AuthenticationChallengeSuccess, credential);
    else
        authmgr->notifyChallengeResult(url, protectionSpace, AuthenticationChallengeCancelled, inputCredential);
}
