void DocumentThreadableLoader::didReceiveResourceTiming(Resource* resource, const ResourceTimingInfo& info)
{
    ASSERT(m_client);
    ASSERT_UNUSED(resource, resource == this->resource());
    ASSERT(m_async);

    m_client->didReceiveResourceTiming(info);
}
