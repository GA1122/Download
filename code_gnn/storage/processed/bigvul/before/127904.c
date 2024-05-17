void DocumentThreadableLoader::handleReceivedData(const char* data, size_t dataLength)
{
    ASSERT(m_client);

    if (!m_actualRequest.isNull())
        return;

    ASSERT(m_fallbackRequestForServiceWorker.isNull());

    m_client->didReceiveData(data, dataLength);
}
