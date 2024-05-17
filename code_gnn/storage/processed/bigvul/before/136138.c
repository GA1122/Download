PassRefPtrWillBeRawPtr<Node> Document::adoptNode(PassRefPtrWillBeRawPtr<Node> source, ExceptionState& exceptionState)
{
    EventQueueScope scope;

    switch (source->nodeType()) {
    case DOCUMENT_NODE:
        exceptionState.throwDOMException(NotSupportedError, "The node provided is of type '" + source->nodeName() + "', which may not be adopted.");
        return nullptr;
    case ATTRIBUTE_NODE: {
        Attr* attr = toAttr(source.get());
        if (RefPtrWillBeRawPtr<Element> ownerElement = attr->ownerElement())
            ownerElement->removeAttributeNode(attr, exceptionState);
        break;
    }
    default:
        if (source->isShadowRoot()) {
            exceptionState.throwDOMException(HierarchyRequestError, "The node provided is a shadow root, which may not be adopted.");
            return nullptr;
        }

        if (source->isFrameOwnerElement()) {
            HTMLFrameOwnerElement* frameOwnerElement = toHTMLFrameOwnerElement(source.get());
            if (frame() && frame()->tree().isDescendantOf(toLocalFrameTemporary(frameOwnerElement->contentFrame()))) {
                exceptionState.throwDOMException(HierarchyRequestError, "The node provided is a frame which contains this document.");
                return nullptr;
            }
        }
        if (source->parentNode()) {
            source->parentNode()->removeChild(source.get(), exceptionState);
            if (exceptionState.hadException())
                return nullptr;
        }
    }

    this->adoptIfNeeded(*source);

    return source;
}
