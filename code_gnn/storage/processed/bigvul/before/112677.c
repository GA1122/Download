void DocumentLoader::maybeFinishLoadingMultipartContent()
{
    if (!isMultipartReplacingLoad())
        return;

    frameLoader()->setupForReplace();
    m_committed = false;
    RefPtr<ResourceBuffer> resourceData = mainResourceData();
    commitLoad(resourceData->data(), resourceData->size());
}
