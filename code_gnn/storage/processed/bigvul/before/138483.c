void HTMLScriptRunner::notifyFinished(Resource* cachedResource)
{
    if (isExecutingScript() && cachedResource->wasCanceled()) {
        stopWatchingResourceForLoad(cachedResource);
        return;
    }
    m_host->notifyScriptLoaded(cachedResource);
}
