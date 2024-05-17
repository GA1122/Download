void DocumentThreadableLoader::notifyFinished(Resource* resource)
{
    ASSERT(m_client);
    ASSERT(resource == this->resource());
    ASSERT(m_async);

    if (resource->errorOccurred()) {
        handleError(resource->resourceError());
    } else {
        handleSuccessfulFinish(resource->identifier(), resource->loadFinishTime());
    }
}
