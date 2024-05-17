void Document::didAddTouchEventHandler(Node* handler)
{
    if (!m_touchEventTargets.get())
        m_touchEventTargets = adoptPtr(new TouchEventTargetSet);
    m_touchEventTargets->add(handler);
    if (Document* parent = parentDocument()) {
        parent->didAddTouchEventHandler(this);
        return;
    }
    if (Page* page = this->page()) {
        if (ScrollingCoordinator* scrollingCoordinator = page->scrollingCoordinator())
            scrollingCoordinator->touchEventTargetRectsDidChange(this);
        if (m_touchEventTargets->size() == 1)
            page->chrome().client().needTouchEvents(true);
    }
}
