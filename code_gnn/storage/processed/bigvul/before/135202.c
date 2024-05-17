void Document::didChangeVisibilityState()
{
    dispatchEvent(Event::createBubble(EventTypeNames::visibilitychange));
    dispatchEvent(Event::createBubble(EventTypeNames::webkitvisibilitychange));

    PageVisibilityState state = pageVisibilityState();
    for (DocumentVisibilityObserver* observer : m_visibilityObservers)
        observer->didChangeVisibilityState(state);

    if (state == PageVisibilityStateVisible)
        timeline().setAllCompositorPending();

    if (hidden() && m_canvasFontCache)
        m_canvasFontCache->pruneAll();
}
