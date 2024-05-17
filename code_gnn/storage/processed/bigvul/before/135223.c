PassRefPtrWillBeRawPtr<HTMLCollection> Document::embeds()
{
    return ensureCachedCollection<HTMLCollection>(DocEmbeds);
}
