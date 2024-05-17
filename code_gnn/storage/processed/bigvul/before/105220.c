Node* StyledMarkupAccumulator::traverseNodesForSerialization(Node* startNode, Node* pastEnd, NodeTraversalMode traversalMode)
{
    const bool shouldEmit = traversalMode == EmitString;
    Vector<Node*> ancestorsToClose;
    Node* next;
    Node* lastClosed = 0;
    for (Node* n = startNode; n != pastEnd; n = next) {
        ASSERT(n);
        if (!n)
            break;
        
        next = n->traverseNextNode();
        bool openedTag = false;

        if (isBlock(n) && canHaveChildrenForEditing(n) && next == pastEnd)
            continue;

        if (!n->renderer() && !enclosingNodeWithTag(firstPositionInOrBeforeNode(n), selectTag)) {
            next = n->traverseNextSibling();
            if (pastEnd && pastEnd->isDescendantOf(n))
                next = pastEnd;
        } else {
            if (shouldEmit)
                appendStartTag(n);

            if (!n->childNodeCount()) {
                if (shouldEmit)
                    appendEndTag(n);
                lastClosed = n;
            } else {
                openedTag = true;
                ancestorsToClose.append(n);
            }
        }

        if (!openedTag && (!n->nextSibling() || next == pastEnd)) {
            while (!ancestorsToClose.isEmpty()) {
                Node* ancestor = ancestorsToClose.last();
                if (next != pastEnd && next->isDescendantOf(ancestor))
                    break;
                if (shouldEmit)
                    appendEndTag(ancestor);
                lastClosed = ancestor;
                ancestorsToClose.removeLast();
            }

            ContainerNode* nextParent = next ? next->parentNode() : 0;
            if (next != pastEnd && n != nextParent) {
                Node* lastAncestorClosedOrSelf = n->isDescendantOf(lastClosed) ? lastClosed : n;
                for (ContainerNode* parent = lastAncestorClosedOrSelf->parentNode(); parent && parent != nextParent; parent = parent->parentNode()) {
                    if (!parent->renderer())
                        continue;
                    ASSERT(startNode->isDescendantOf(parent));
                    if (shouldEmit)
                        wrapWithNode(parent);
                    lastClosed = parent;
                }
            }
        }
    }

    return lastClosed;
}
