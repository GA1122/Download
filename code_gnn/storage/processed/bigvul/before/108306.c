void MainResourceLoader::handleDataLoadSoon(ResourceRequest& r)
{
    m_initialRequest = r;
    
    if (m_documentLoader->deferMainResourceDataLoad())
        startDataLoadTimer();
    else
        handleDataLoadNow(0);
}
