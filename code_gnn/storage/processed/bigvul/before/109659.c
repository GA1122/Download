PassRefPtr<HTMLCollection> Document::embeds()
{
    return ensureCachedCollection(DocEmbeds);
}
