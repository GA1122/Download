void WebPageProxy::exceededDatabaseQuota(uint64_t frameID, const String& originIdentifier, const String& databaseName, const String& displayName, uint64_t currentQuota, uint64_t currentUsage, uint64_t expectedUsage, uint64_t& newQuota)
{
    WebFrameProxy* frame = process()->webFrame(frameID);
    MESSAGE_CHECK(frame);

    RefPtr<WebSecurityOrigin> origin = WebSecurityOrigin::create(originIdentifier);

    newQuota = m_uiClient.exceededDatabaseQuota(this, frame, origin.get(), databaseName, displayName, currentQuota, currentUsage, expectedUsage);
}
