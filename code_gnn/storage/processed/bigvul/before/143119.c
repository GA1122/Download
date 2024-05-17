Node* Document::adoptNode(Node* source, ExceptionState& exceptionState)
{
    EventQueueScope scope;

    switch (source->getNodeType()) {
    case DOCUMENT_NODE:
        exceptionState.throwDOMException(NotSupportedError, "The node provided is of type '" + source->nodeName() + "', which may not be adopted.");
        return nullptr;
    case ATTRIBUTE_NODE: {
        Attr* attr = toAttr(source);
        if (Element* ownerElement = attr->ownerElement())
            ownerElement->removeAttributeNode(attr, exceptionState);
        break;
    }
    default:
        if (source->isShadowRoot()) {
            exceptionState.throwDOMException(HierarchyRequestError, "The node provided is a shadow root, which may not be adopted.");
            return nullptr;
        }

        if (source->isFrameOwnerElement()) {
            HTMLFrameOwnerElement* frameOwnerElement = toHTMLFrameOwnerElement(source);
            if (frame() && frame()->tree().isDescendantOf(frameOwnerElement->contentFrame())) {
                exceptionState.throwDOMException(HierarchyRequestError, "The node provided is a frame which contains this document.");
                return nullptr;
            }
        }
        if (source->parentNode()) {
            source->parentNode()->removeChild(source, exceptionState);
            if (exceptionState.hadException())
                return nullptr;
            RELEASE_ASSERT(!source->parentNode());
        }
    }

    this->adoptIfNeeded(*source);

    return source;
}
