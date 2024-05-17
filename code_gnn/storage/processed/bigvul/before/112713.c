void DocumentLoader::substituteResourceDeliveryTimerFired(Timer<DocumentLoader>*)
{
    if (m_pendingSubstituteResources.isEmpty())
        return;
    ASSERT(m_frame && m_frame->page());
    if (m_frame->page()->defersLoading())
        return;

    SubstituteResourceMap copy;
    copy.swap(m_pendingSubstituteResources);

    SubstituteResourceMap::const_iterator end = copy.end();
    for (SubstituteResourceMap::const_iterator it = copy.begin(); it != end; ++it) {
        RefPtr<ResourceLoader> loader = it->key;
        SubstituteResource* resource = it->value.get();
        
        if (resource) {
            SharedBuffer* data = resource->data();
        
            loader->didReceiveResponse(resource->response());

            if (loader->reachedTerminalState())
                return;

            loader->didReceiveData(data->data(), data->size(), data->size(), DataPayloadWholeResource);

            if (loader->reachedTerminalState())
                return;

            loader->didFinishLoading(0);
        } else {
            loader->didFail(loader->cannotShowURLError());
        }
    }
}
