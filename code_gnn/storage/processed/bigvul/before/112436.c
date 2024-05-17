void Document::didAddTouchEventHandler(Node* handler)
{
#if ENABLE(TOUCH_EVENTS)
    if (!m_touchEventTargets.get())
        m_touchEventTargets = adoptPtr(new TouchEventTargetSet);
    m_touchEventTargets->add(handler);
    if (Document* parent = parentDocument()) {
        parent->didAddTouchEventHandler(this);
        return;
    }
    if (Page* page = this->page()) {
#if ENABLE(TOUCH_EVENT_TRACKING)
        if (ScrollingCoordinator* scrollingCoordinator = page->scrollingCoordinator())
            scrollingCoordinator->touchEventTargetRectsDidChange(this);
#endif
        if (m_touchEventTargets->size() == 1)
            page->chrome()->client()->needTouchEvents(true);
    }
#else
    UNUSED_PARAM(handler);
#endif
}
