void Document::didRemoveTouchEventHandler(Node* handler)
{
#if ENABLE(TOUCH_EVENTS)
    if (!m_touchEventTargets.get())
        return;
    ASSERT(m_touchEventTargets->contains(handler));
    m_touchEventTargets->remove(handler);
    if (Document* parent = parentDocument()) {
        parent->didRemoveTouchEventHandler(this);
        return;
    }

    Page* page = this->page();
    if (!page)
        return;
#if ENABLE(TOUCH_EVENT_TRACKING)
    if (ScrollingCoordinator* scrollingCoordinator = page->scrollingCoordinator())
        scrollingCoordinator->touchEventTargetRectsDidChange(this);
#endif
    if (m_touchEventTargets->size())
        return;
    for (const Frame* frame = page->mainFrame(); frame; frame = frame->tree()->traverseNext()) {
        if (frame->document() && frame->document()->hasTouchEventHandlers())
            return;
    }
    page->chrome()->client()->needTouchEvents(false);
#else
    UNUSED_PARAM(handler);
#endif
}
