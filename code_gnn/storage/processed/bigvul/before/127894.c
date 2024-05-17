void DocumentThreadableLoader::dataReceived(Resource* resource, const char* data, size_t dataLength)
{
    ASSERT_UNUSED(resource, resource == this->resource());
    ASSERT(m_async);

    if (m_isUsingDataConsumerHandle)
        return;

    handleReceivedData(data, safeCast<unsigned>(dataLength));
}
