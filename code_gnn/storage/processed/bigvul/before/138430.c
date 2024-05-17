static WeakDocumentSet& liveDocumentSet()
{
    DEFINE_STATIC_LOCAL(OwnPtrWillBePersistent<WeakDocumentSet>, set, (adoptPtrWillBeNoop(new WeakDocumentSet())));
    return *set;
}