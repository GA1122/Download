void FrameLoader::loadProvisionalItemFromCachedPage()
{
    DocumentLoader* provisionalLoader = provisionalDocumentLoader();
    LOG(PageCache, "WebCorePageCache: Loading provisional DocumentLoader %p with URL '%s' from CachedPage", provisionalDocumentLoader(), provisionalDocumentLoader()->url().string().utf8().data());

    provisionalLoader->prepareForLoadStart();

    m_loadingFromCachedPage = true;

    provisionalLoader->setCommitted(true);
    commitProvisionalLoad();
}
