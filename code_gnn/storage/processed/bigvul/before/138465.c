DocumentVisibilityObserver::~DocumentVisibilityObserver()
{
#if !ENABLE(OILPAN)
    unregisterObserver();
#endif
}
