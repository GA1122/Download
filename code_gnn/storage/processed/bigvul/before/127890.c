void DocumentThreadableLoader::cancelWithError(const ResourceError& error)
{
    if (!m_client || !resource()) {
        clear();
        return;
    }

    ResourceError errorForCallback = error;
    if (errorForCallback.isNull()) {
        errorForCallback = ResourceError(errorDomainBlinkInternal, 0, resource()->url().getString(), "Load cancelled");
        errorForCallback.setIsCancellation(true);
    }

    ThreadableLoaderClient* client = m_client;
    clear();
    client->didFail(errorForCallback);
}
