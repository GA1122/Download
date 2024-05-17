void ResourceFetcher::didReceiveData(const Resource* resource, const char* data, int dataLength, int encodedDataLength)
{
    context().dispatchDidReceiveData(m_documentLoader, resource->identifier(), data, dataLength, encodedDataLength);
}
