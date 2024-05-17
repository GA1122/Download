PageClientBlackBerry::SaveCredentialType WebPagePrivate::notifyShouldSaveCredential(bool isNew)
{
    return static_cast<PageClientBlackBerry::SaveCredentialType>(m_client->notifyShouldSaveCredential(isNew));
}
