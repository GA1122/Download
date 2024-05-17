ResourceLoadTiming* Timing::resourceLoadTiming() const
{
    DocumentLoader* loader = documentLoader();
    if (!loader)
        return 0;

    return loader->response().resourceLoadTiming();
}
