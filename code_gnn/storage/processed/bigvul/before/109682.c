PassRefPtr<HTMLCollection> Document::forms()
{
    return ensureCachedCollection(DocForms);
}
