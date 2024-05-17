void Document::updateHoverActiveState(const HitTestRequest& request, Element* innerElement)
{
    ASSERT(!request.readOnly());

    Element* innerElementInDocument = innerElement;
    while (innerElementInDocument && innerElementInDocument->document() != this) {
        innerElementInDocument->document()->updateHoverActiveState(request, innerElementInDocument);
        innerElementInDocument = innerElementInDocument->document()->ownerElement();
    }

    Element* oldActiveElement = activeElement();
    if (oldActiveElement && !request.active()) {
        for (RenderObject* curr = oldActiveElement->renderer(); curr; curr = curr->parent()) {
            if (curr->node()) {
                ASSERT(!curr->node()->isTextNode());
                curr->node()->setActive(false);
                m_userActionElements.setInActiveChain(curr->node(), false);
            }
        }
        setActiveElement(0);
    } else {
        Element* newActiveElement = innerElementInDocument;
        if (!oldActiveElement && newActiveElement && request.active() && !request.touchMove()) {
            for (RenderObject* curr = newActiveElement->renderer(); curr; curr = curr->parent()) {
                if (curr->node() && !curr->isText())
                    m_userActionElements.setInActiveChain(curr->node(), true);
            }

            setActiveElement(newActiveElement);
        }
    }
    bool allowActiveChanges = !oldActiveElement && activeElement();

    bool mustBeInActiveChain = request.active() && request.move();

    RefPtr<Node> oldHoverNode = hoverNode();

    if (request.touchRelease())
        innerElementInDocument = 0;

    Node* newHoverNode = innerElementInDocument;
    while (newHoverNode && !newHoverNode->renderer())
        newHoverNode = newHoverNode->parentOrShadowHostNode();

    setHoverNode(newHoverNode);

    RenderObject* oldHoverObj = oldHoverNode ? oldHoverNode->renderer() : 0;
    RenderObject* newHoverObj = newHoverNode ? newHoverNode->renderer() : 0;

    RenderObject* ancestor = nearestCommonHoverAncestor(oldHoverObj, newHoverObj);

    Vector<RefPtr<Node>, 32> nodesToRemoveFromChain;
    Vector<RefPtr<Node>, 32> nodesToAddToChain;

    if (oldHoverObj != newHoverObj) {
        for (RenderObject* curr = oldHoverObj; curr && curr != ancestor; curr = curr->hoverAncestor()) {
            if (curr->node() && !curr->isText() && (!mustBeInActiveChain || curr->node()->inActiveChain()))
                nodesToRemoveFromChain.append(curr->node());
        }
    }

    for (RenderObject* curr = newHoverObj; curr; curr = curr->hoverAncestor()) {
        if (curr->node() && !curr->isText() && (!mustBeInActiveChain || curr->node()->inActiveChain()))
            nodesToAddToChain.append(curr->node());
    }

    size_t removeCount = nodesToRemoveFromChain.size();
    for (size_t i = 0; i < removeCount; ++i)
        nodesToRemoveFromChain[i]->setHovered(false);

    size_t addCount = nodesToAddToChain.size();
    for (size_t i = 0; i < addCount; ++i) {
        if (allowActiveChanges)
            nodesToAddToChain[i]->setActive(true);
        nodesToAddToChain[i]->setHovered(true);
    }

    updateStyleIfNeeded();
}
