static bool shouldComplete(Document* document)
{
    if (!document->frame())
        return false;
    if (document->parsing() || document->isInDOMContentLoaded())
        return false;
    if (!document->haveImportsLoaded())
        return false;
    if (document->fetcher()->requestCount())
        return false;
    if (document->isDelayingLoadEvent())
        return false;
    return allDescendantsAreComplete(document->frame());
}
