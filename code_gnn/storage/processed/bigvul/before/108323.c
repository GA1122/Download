DocumentLoadTiming* Timing::documentLoadTiming() const
{
    DocumentLoader* loader = documentLoader();
    if (!loader)
        return 0;

    return loader->timing();
}
