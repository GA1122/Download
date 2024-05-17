bool Document::setFocusedElement(Element* prpNewFocusedElement, const FocusParams& params)
{
    DCHECK(!m_lifecycle.inDetach());

    m_clearFocusedElementTimer.stop();

    Element* newFocusedElement = prpNewFocusedElement;

    if (newFocusedElement && (newFocusedElement->document() != this))
        return true;

    if (NodeChildRemovalTracker::isBeingRemoved(newFocusedElement))
        return true;

    if (m_focusedElement == newFocusedElement)
        return true;

    bool focusChangeBlocked = false;
    Element* oldFocusedElement = m_focusedElement;
    m_focusedElement = nullptr;

    if (oldFocusedElement) {
        oldFocusedElement->setFocus(false);

        if (page() && (page()->focusController().isFocused())) {
            oldFocusedElement->dispatchBlurEvent(newFocusedElement, params.type, params.sourceCapabilities);

            if (m_focusedElement) {
                focusChangeBlocked = true;
                newFocusedElement = nullptr;
            }

            oldFocusedElement->dispatchFocusOutEvent(EventTypeNames::focusout, newFocusedElement, params.sourceCapabilities);  
            oldFocusedElement->dispatchFocusOutEvent(EventTypeNames::DOMFocusOut, newFocusedElement, params.sourceCapabilities);  

            if (m_focusedElement) {
                focusChangeBlocked = true;
                newFocusedElement = nullptr;
            }
        }

        if (view()) {
            Widget* oldWidget = widgetForElement(*oldFocusedElement);
            if (oldWidget)
                oldWidget->setFocus(false, params.type);
            else
                view()->setFocus(false, params.type);
        }
    }

    if (newFocusedElement)
        updateStyleAndLayoutTreeForNode(newFocusedElement);
    if (newFocusedElement && newFocusedElement->isFocusable()) {
        if (newFocusedElement->isRootEditableElement() && !acceptsEditingFocus(*newFocusedElement)) {
            focusChangeBlocked = true;
            goto SetFocusedElementDone;
        }
        m_focusedElement = newFocusedElement;
        setSequentialFocusNavigationStartingPoint(m_focusedElement.get());

        m_focusedElement->setFocus(true);
        if (m_focusedElement != newFocusedElement) {
            focusChangeBlocked = true;
            goto SetFocusedElementDone;
        }
        cancelFocusAppearanceUpdate();
        m_focusedElement->updateFocusAppearance(params.selectionBehavior);

        if (page() && (page()->focusController().isFocused())) {
            m_focusedElement->dispatchFocusEvent(oldFocusedElement, params.type, params.sourceCapabilities);


            if (m_focusedElement != newFocusedElement) {
                focusChangeBlocked = true;
                goto SetFocusedElementDone;
            }
            m_focusedElement->dispatchFocusInEvent(EventTypeNames::focusin, oldFocusedElement, params.type, params.sourceCapabilities);  

            if (m_focusedElement != newFocusedElement) {
                focusChangeBlocked = true;
                goto SetFocusedElementDone;
            }

            m_focusedElement->dispatchFocusInEvent(EventTypeNames::DOMFocusIn, oldFocusedElement, params.type, params.sourceCapabilities);  

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
                updateStyleAndLayout();
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
            cache->handleFocusedUIElementChanged(oldFocusedElement, newFocusedElement);
    }

    if (!focusChangeBlocked && frameHost())
        frameHost()->chromeClient().focusedNodeChanged(oldFocusedElement, m_focusedElement.get());

SetFocusedElementDone:
    updateStyleAndLayoutTree();
    if (LocalFrame* frame = this->frame())
        frame->selection().didChangeFocus();
    return !focusChangeBlocked;
}
