bool Document::setFocusedNode(PassRefPtr<Node> prpNewFocusedNode, FocusDirection direction)
{
    RefPtr<Node> newFocusedNode = prpNewFocusedNode;

    if (newFocusedNode && (newFocusedNode->document() != this))
        return true;

    if (m_focusedNode == newFocusedNode)
        return true;

    if (m_inPageCache)
        return false;

    bool focusChangeBlocked = false;
    RefPtr<Node> oldFocusedNode = m_focusedNode;
    m_focusedNode = 0;

    if (oldFocusedNode) {
        ASSERT(!oldFocusedNode->inDetach());

        if (oldFocusedNode->active())
            oldFocusedNode->setActive(false);

        oldFocusedNode->setFocus(false);

        if (oldFocusedNode->isElementNode()) {
            Element* element = toElement(oldFocusedNode.get());
            if (element->wasChangedSinceLastFormControlChangeEvent())
                element->dispatchFormControlChangeEvent();
        }

        oldFocusedNode->dispatchBlurEvent(newFocusedNode);

        if (m_focusedNode) {
            focusChangeBlocked = true;
            newFocusedNode = 0;
        }
        
        oldFocusedNode->dispatchFocusOutEvent(eventNames().focusoutEvent, newFocusedNode);  
        oldFocusedNode->dispatchFocusOutEvent(eventNames().DOMFocusOutEvent, newFocusedNode);  

        if (m_focusedNode) {
            focusChangeBlocked = true;
            newFocusedNode = 0;
        }
        if (oldFocusedNode == this && oldFocusedNode->hasOneRef())
            return true;
            
        if (oldFocusedNode->isRootEditableElement())
            frame()->editor()->didEndEditing();

        if (view()) {
            Widget* oldWidget = widgetForNode(oldFocusedNode.get());
            if (oldWidget)
                oldWidget->setFocus(false);
            else
                view()->setFocus(false);
        }
    }

    if (newFocusedNode && (newFocusedNode->isPluginElement() || newFocusedNode->isFocusable())) {
        if (newFocusedNode->isRootEditableElement() && !acceptsEditingFocus(newFocusedNode.get())) {
            focusChangeBlocked = true;
            goto SetFocusedNodeDone;
        }
        m_focusedNode = newFocusedNode;

        m_focusedNode->dispatchFocusEvent(oldFocusedNode, direction);

        if (m_focusedNode != newFocusedNode) {
            focusChangeBlocked = true;
            goto SetFocusedNodeDone;
        }

        m_focusedNode->dispatchFocusInEvent(eventNames().focusinEvent, oldFocusedNode);  

        if (m_focusedNode != newFocusedNode) {
            focusChangeBlocked = true;
            goto SetFocusedNodeDone;
        }

        m_focusedNode->dispatchFocusInEvent(eventNames().DOMFocusInEvent, oldFocusedNode);  

        if (m_focusedNode != newFocusedNode) { 
            focusChangeBlocked = true;
            goto SetFocusedNodeDone;
        }
        m_focusedNode->setFocus(true);

        if (m_focusedNode->isRootEditableElement())
            frame()->editor()->didBeginEditing();

        if (view()) {
            Widget* focusWidget = widgetForNode(m_focusedNode.get());
            if (focusWidget) {
                updateLayout();
                focusWidget = widgetForNode(m_focusedNode.get());
            }
            if (focusWidget)
                focusWidget->setFocus(true);
            else
                view()->setFocus(true);
        }
    }

    if (!focusChangeBlocked && m_focusedNode) {
        if (AXObjectCache* cache = axObjectCache())
            cache->handleFocusedUIElementChanged(oldFocusedNode.get(), newFocusedNode.get());
    }

    if (!focusChangeBlocked)
        page()->chrome()->focusedNodeChanged(m_focusedNode.get());

SetFocusedNodeDone:
    updateStyleIfNeeded();
    return !focusChangeBlocked;
}
