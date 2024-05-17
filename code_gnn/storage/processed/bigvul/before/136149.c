void Document::didChangeVisibilityState()
{
    dispatchEvent(Event::create(EventTypeNames::visibilitychange));
    dispatchEvent(Event::create(EventTypeNames::webkitvisibilitychange));

    PageVisibilityState state = pageVisibilityState();
    for (DocumentVisibilityObserver* observer : m_visibilityObservers)
        observer->didChangeVisibilityState(state);

    if (state == PageVisibilityStateVisible)
        timeline().setAllCompositorPending();

    if (hidden() && m_canvasFontCache)
        m_canvasFontCache->pruneAll();
}
