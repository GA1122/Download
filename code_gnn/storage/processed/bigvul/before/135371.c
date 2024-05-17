bool Document::setFocusedElement(PassRefPtrWillBeRawPtr<Element> prpNewFocusedElement, const FocusParams& params)
{
    ASSERT(!m_lifecycle.inDetach());

    m_clearFocusedElementTimer.stop();

    RefPtrWillBeRawPtr<Element> newFocusedElement = prpNewFocusedElement;

    if (newFocusedElement && (newFocusedElement->document() != this))
        return true;

    if (NodeChildRemovalTracker::isBeingRemoved(newFocusedElement.get()))
        return true;

    if (m_focusedElement == newFocusedElement)
        return true;

    bool focusChangeBlocked = false;
    RefPtrWillBeRawPtr<Element> oldFocusedElement = m_focusedElement;
    m_focusedElement = nullptr;

    if (oldFocusedElement) {
        if (oldFocusedElement->active())
            oldFocusedElement->setActive(false);

        oldFocusedElement->setFocus(false);

        if (page() && (page()->focusController().isFocused())) {
            oldFocusedElement->dispatchBlurEvent(newFocusedElement.get(), params.type, params.sourceCapabilities);

            if (m_focusedElement) {
                focusChangeBlocked = true;
                newFocusedElement = nullptr;
            }

            oldFocusedElement->dispatchFocusOutEvent(EventTypeNames::focusout, newFocusedElement.get(), params.sourceCapabilities);  
            oldFocusedElement->dispatchFocusOutEvent(EventTypeNames::DOMFocusOut, newFocusedElement.get(), params.sourceCapabilities);  

            if (m_focusedElement) {
                focusChangeBlocked = true;
                newFocusedElement = nullptr;
            }
            if (newFocusedElement)
                updateLayoutTreeIgnorePendingStylesheets();
        }

        if (view()) {
            Widget* oldWidget = widgetForElement(*oldFocusedElement);
            if (oldWidget)
                oldWidget->setFocus(false, params.type);
            else
                view()->setFocus(false, params.type);
        }
    }

    if (newFocusedElement && newFocusedElement->isFocusable()) {
        if (newFocusedElement->isRootEditableElement() && !acceptsEditingFocus(*newFocusedElement)) {
            focusChangeBlocked = true;
            goto SetFocusedElementDone;
        }
        m_focusedElement = newFocusedElement;

        m_focusedElement->setFocus(true);
        cancelFocusAppearanceUpdate();
        m_focusedElement->updateFocusAppearance(params.selectionBehavior);

        if (page() && (page()->focusController().isFocused())) {
            m_focusedElement->dispatchFocusEvent(oldFocusedElement.get(), params.type, params.sourceCapabilities);


            if (m_focusedElement != newFocusedElement) {
                focusChangeBlocked = true;
                goto SetFocusedElementDone;
            }
            m_focusedElement->dispatchFocusInEvent(EventTypeNames::focusin, oldFocusedElement.get(), params.type, params.sourceCapabilities);  

            if (m_focusedElement != newFocusedElement) {
                focusChangeBlocked = true;
                goto SetFocusedElementDone;
            }

            m_focusedElement->dispatchFocusInEvent(EventTypeNames::DOMFocusIn, oldFocusedElement.get(), params.type, params.sourceCapabilities);  

            if (m_focusedElement != newFocusedElement) {
                focusChangeBlocked = true;
                goto SetFocusedElementDone;
            }
        }

        if (m_focusedElement->isRootEditableElement())
            frame()->spellChecker().didBeginEditing(m_focusedElement.get());

        if (view()) {
            Widget* focusWidget = widgetForElement(*m_focusedElement);
            if (focusWidget) {
                updateLayout();
                focusWidget = widgetForElement(*m_focusedElement);
            }
            if (focusWidget)
                focusWidget->setFocus(true, params.type);
            else
                view()->setFocus(true, params.type);
        }
    }

    if (!focusChangeBlocked && m_focusedElement) {
        if (AXObjectCache* cache = axObjectCache())
            cache->handleFocusedUIElementChanged(oldFocusedElement.get(), newFocusedElement.get());
    }

    if (!focusChangeBlocked && frameHost())
        frameHost()->chromeClient().focusedNodeChanged(oldFocusedElement.get(), m_focusedElement.get());

SetFocusedElementDone:
    updateLayoutTreeIfNeeded();
    if (LocalFrame* frame = this->frame())
        frame->selection().didChangeFocus();
    return !focusChangeBlocked;
}
