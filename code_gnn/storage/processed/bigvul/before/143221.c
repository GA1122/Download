static WeakDocumentSet& liveDocumentSet()
{
    DEFINE_STATIC_LOCAL(WeakDocumentSet, set, ());
    return set;
}
