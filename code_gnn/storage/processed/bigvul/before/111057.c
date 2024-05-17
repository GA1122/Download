IntRect WebPagePrivate::adjustRectOffsetForFrameOffset(const IntRect& rect, const Node* node)
{
    if (!node)
        return rect;

    const Node* tnode = node;
    IntRect adjustedRect = rect;
    do {
        Frame* frame = tnode->document()->frame();
        if (!frame)
            continue;

        Node* ownerNode = static_cast<Node*>(frame->ownerElement());
        tnode = ownerNode;
        if (ownerNode && (ownerNode->hasTagName(HTMLNames::iframeTag) || ownerNode->hasTagName(HTMLNames::frameTag))) {
            IntRect iFrameRect;
            do {
                iFrameRect = rectForNode(ownerNode);
                adjustedRect.move(iFrameRect.x(), iFrameRect.y());
                adjustedRect.intersect(iFrameRect);
                ownerNode = ownerNode->parentNode();
            } while (iFrameRect.isEmpty() && ownerNode);
        } else
            break;
    } while (tnode = tnode->parentNode());

    return adjustedRect;
}
