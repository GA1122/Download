void Document::updateURLForPushOrReplaceState(const KURL& url)
{
    Frame* f = frame();
    if (!f)
        return;

    setURL(url);
    f->loader()->setOutgoingReferrer(url);

    if (DocumentLoader* documentLoader = loader())
        documentLoader->replaceRequestURLForSameDocumentNavigation(url);
}
