PassRefPtr<Node> Document::adoptNode(PassRefPtr<Node> source, ExceptionCode& ec)
{
    if (!source) {
        ec = NOT_SUPPORTED_ERR;
        return 0;
    }

    if (source->isReadOnlyNode()) {
        ec = NO_MODIFICATION_ALLOWED_ERR;
        return 0;
    }

    EventQueueScope scope;

    switch (source->nodeType()) {
    case ENTITY_NODE:
    case NOTATION_NODE:
    case DOCUMENT_NODE:
    case DOCUMENT_TYPE_NODE:
    case XPATH_NAMESPACE_NODE:
        ec = NOT_SUPPORTED_ERR;
        return 0;            
    case ATTRIBUTE_NODE: {                   
        Attr* attr = static_cast<Attr*>(source.get());
        if (attr->ownerElement())
            attr->ownerElement()->removeAttributeNode(attr, ec);
        attr->setSpecified(true);
        break;
    }       
    default:
        if (source->isShadowRoot()) {
            ec = HIERARCHY_REQUEST_ERR;
            return 0;
        }

        if (source->isFrameOwnerElement()) {
            HTMLFrameOwnerElement* frameOwnerElement = toFrameOwnerElement(source.get());
            if (frame() && frame()->tree()->isDescendantOf(frameOwnerElement->contentFrame())) {
                ec = HIERARCHY_REQUEST_ERR;
                return 0;
            }
        }
        if (source->parentNode()) {
            source->parentNode()->removeChild(source.get(), ec);
            if (ec)
                return 0;
        }
    }

    this->adoptIfNeeded(source.get());

    return source;
}
