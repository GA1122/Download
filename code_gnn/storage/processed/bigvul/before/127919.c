void DocumentThreadableLoader::setSerializedCachedMetadata(Resource*, const char* data, size_t size)
{
    if (!m_actualRequest.isNull())
        return;
    m_client->didReceiveCachedMetadata(data, size);
}
