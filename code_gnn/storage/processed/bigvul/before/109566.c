PassRefPtr<Node> Document::adoptNode(PassRefPtr<Node> source, ExceptionState& es)
{
    if (!source) {
        es.throwUninformativeAndGenericDOMException(NotSupportedError);
        return 0;
    }

    EventQueueScope scope;

    switch (source->nodeType()) {
    case ENTITY_NODE:
    case NOTATION_NODE:
    case DOCUMENT_NODE:
    case DOCUMENT_TYPE_NODE:
    case XPATH_NAMESPACE_NODE:
        es.throwUninformativeAndGenericDOMException(NotSupportedError);
        return 0;
    case ATTRIBUTE_NODE: {
        Attr* attr = toAttr(source.get());
        if (attr->ownerElement())
            attr->ownerElement()->removeAttributeNode(attr, es);
        break;
    }
    default:
        if (source->isShadowRoot()) {
            es.throwUninformativeAndGenericDOMException(HierarchyRequestError);
            return 0;
        }

        if (source->isFrameOwnerElement()) {
            HTMLFrameOwnerElement* frameOwnerElement = toHTMLFrameOwnerElement(source.get());
            if (frame() && frame()->tree()->isDescendantOf(frameOwnerElement->contentFrame())) {
                es.throwUninformativeAndGenericDOMException(HierarchyRequestError);
                return 0;
            }
        }
        if (source->parentNode()) {
            source->parentNode()->removeChild(source.get(), es);
            if (es.hadException())
                return 0;
        }
    }

    this->adoptIfNeeded(source.get());

    return source;
}
