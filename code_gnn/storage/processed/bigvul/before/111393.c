void WebPagePrivate::syncProxyCredential(const WebCore::Credential& credential)
{
    m_client->syncProxyCredential(credential.user(), credential.password());
}
