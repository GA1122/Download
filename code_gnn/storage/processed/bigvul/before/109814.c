bool Document::setFocusedElement(PassRefPtr<Element> prpNewFocusedElement, FocusDirection direction)
{
    RefPtr<Element> newFocusedElement = prpNewFocusedElement;

    if (newFocusedElement && (newFocusedElement->document() != this))
        return true;

    if (NodeChildRemovalTracker::isBeingRemoved(newFocusedElement.get()))
        return true;

    if (m_focusedElement == newFocusedElement)
        return true;

    bool focusChangeBlocked = false;
    RefPtr<Element> oldFocusedElement = m_focusedElement;
    m_focusedElement = 0;

    if (oldFocusedElement) {
        ASSERT(!oldFocusedElement->inDetach());

        if (oldFocusedElement->active())
            oldFocusedElement->setActive(false);

        oldFocusedElement->setFocus(false);

        if (oldFocusedElement->wasChangedSinceLastFormControlChangeEvent())
            oldFocusedElement->dispatchFormControlChangeEvent();

        oldFocusedElement->dispatchBlurEvent(newFocusedElement.get());

        if (m_focusedElement) {
            focusChangeBlocked = true;
            newFocusedElement = 0;
        }

        oldFocusedElement->dispatchFocusOutEvent(EventTypeNames::focusout, newFocusedElement.get());  
        oldFocusedElement->dispatchFocusOutEvent(EventTypeNames::DOMFocusOut, newFocusedElement.get());  

        if (m_focusedElement) {
            focusChangeBlocked = true;
            newFocusedElement = 0;
        }

        if (view()) {
            Widget* oldWidget = widgetForElement(oldFocusedElement.get());
            if (oldWidget)
                oldWidget->setFocus(false);
            else
                view()->setFocus(false);
        }

        updateLayoutIgnorePendingStylesheets();
    }

    if (newFocusedElement && newFocusedElement->isFocusable()) {
        if (newFocusedElement->isRootEditableElement() && !acceptsEditingFocus(newFocusedElement.get())) {
            focusChangeBlocked = true;
            goto SetFocusedElementDone;
        }
        m_focusedElement = newFocusedElement;

        m_focusedElement->dispatchFocusEvent(oldFocusedElement.get(), direction);

        if (m_focusedElement != newFocusedElement) {
            focusChangeBlocked = true;
            goto SetFocusedElementDone;
        }

        m_focusedElement->dispatchFocusInEvent(EventTypeNames::focusin, oldFocusedElement.get());  

        if (m_focusedElement != newFocusedElement) {
            focusChangeBlocked = true;
            goto SetFocusedElementDone;
        }

        m_focusedElement->dispatchFocusInEvent(EventTypeNames::DOMFocusIn, oldFocusedElement.get());  

        if (m_focusedElement != newFocusedElement) {
            focusChangeBlocked = true;
            goto SetFocusedElementDone;
        }
        m_focusedElement->setFocus(true);

        if (m_focusedElement->isRootEditableElement())
            frame()->editor().didBeginEditing(m_focusedElement.get());

        if (view()) {
            Widget* focusWidget = widgetForElement(m_focusedElement.get());
            if (focusWidget) {
                updateLayout();
                focusWidget = widgetForElement(m_focusedElement.get());
            }
            if (focusWidget)
                focusWidget->setFocus(true);
            else
                view()->setFocus(true);
        }
    }

    if (!focusChangeBlocked && m_focusedElement) {
        if (AXObjectCache* cache = axObjectCache())
            cache->handleFocusedUIElementChanged(oldFocusedElement.get(), newFocusedElement.get());
    }

    if (!focusChangeBlocked && page())
        page()->chrome().focusedNodeChanged(m_focusedElement.get());

SetFocusedElementDone:
    updateStyleIfNeeded();
    if (Frame* frame = this->frame())
        frame->selection().didChangeFocus();
    return !focusChangeBlocked;
}
