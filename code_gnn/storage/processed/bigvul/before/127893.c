void DocumentThreadableLoader::dataDownloaded(Resource* resource, int dataLength)
{
    ASSERT(m_client);
    ASSERT_UNUSED(resource, resource == this->resource());
    ASSERT(m_actualRequest.isNull());
    ASSERT(m_async);

    m_client->didDownloadData(dataLength);
}
