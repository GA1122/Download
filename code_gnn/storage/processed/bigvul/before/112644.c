void DocumentLoader::clearMainResourceLoader()
{
    m_loadingMainResource = false;
    if (this == frameLoader()->activeDocumentLoader())
        checkLoadComplete();
}
