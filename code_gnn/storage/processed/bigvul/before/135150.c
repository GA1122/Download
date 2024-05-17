bool Document::canAcceptChild(const Node& newChild, const Node* oldChild, ExceptionState& exceptionState) const
{
    if (oldChild && oldChild->nodeType() == newChild.nodeType())
        return true;

    int numDoctypes = 0;
    int numElements = 0;

    for (Node& child : NodeTraversal::childrenOf(*this)) {
        if (oldChild && *oldChild == child)
            continue;

        switch (child.nodeType()) {
        case DOCUMENT_TYPE_NODE:
            numDoctypes++;
            break;
        case ELEMENT_NODE:
            numElements++;
            break;
        default:
            break;
        }
    }

    if (newChild.isDocumentFragment()) {
        for (Node& child : NodeTraversal::childrenOf(toDocumentFragment(newChild))) {
            switch (child.nodeType()) {
            case ATTRIBUTE_NODE:
            case CDATA_SECTION_NODE:
            case DOCUMENT_FRAGMENT_NODE:
            case DOCUMENT_NODE:
            case TEXT_NODE:
                exceptionState.throwDOMException(HierarchyRequestError, "Nodes of type '" + newChild.nodeName() +
                    "' may not be inserted inside nodes of type '#document'.");
                return false;
            case COMMENT_NODE:
            case PROCESSING_INSTRUCTION_NODE:
                break;
            case DOCUMENT_TYPE_NODE:
                numDoctypes++;
                break;
            case ELEMENT_NODE:
                numElements++;
                break;
            }
        }
    } else {
        switch (newChild.nodeType()) {
        case ATTRIBUTE_NODE:
        case CDATA_SECTION_NODE:
        case DOCUMENT_FRAGMENT_NODE:
        case DOCUMENT_NODE:
        case TEXT_NODE:
            exceptionState.throwDOMException(HierarchyRequestError, "Nodes of type '" + newChild.nodeName() +
                "' may not be inserted inside nodes of type '#document'.");
            return false;
        case COMMENT_NODE:
        case PROCESSING_INSTRUCTION_NODE:
            return true;
        case DOCUMENT_TYPE_NODE:
            numDoctypes++;
            break;
        case ELEMENT_NODE:
            numElements++;
            break;
        }
    }

    if (numElements > 1 || numDoctypes > 1) {
        exceptionState.throwDOMException(HierarchyRequestError,
            String::format("Only one %s on document allowed.",
            numElements > 1 ? "element" : "doctype"));
        return false;
    }

    return true;
}
