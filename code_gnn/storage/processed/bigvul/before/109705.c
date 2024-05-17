PassRefPtr<Node> Document::importNode(Node* importedNode, bool deep, ExceptionState& es)
{
    if (!importedNode) {
        es.throwUninformativeAndGenericDOMException(NotSupportedError);
        return 0;
    }

    switch (importedNode->nodeType()) {
    case TEXT_NODE:
        return createTextNode(importedNode->nodeValue());
    case CDATA_SECTION_NODE:
        return createCDATASection(importedNode->nodeValue(), es);
    case PROCESSING_INSTRUCTION_NODE:
        return createProcessingInstruction(importedNode->nodeName(), importedNode->nodeValue(), es);
    case COMMENT_NODE:
        return createComment(importedNode->nodeValue());
    case ELEMENT_NODE: {
        Element* oldElement = toElement(importedNode);
        if (!hasValidNamespaceForElements(oldElement->tagQName())) {
            es.throwUninformativeAndGenericDOMException(NamespaceError);
            return 0;
        }
        RefPtr<Element> newElement = createElement(oldElement->tagQName(), false);

        newElement->cloneDataFromElement(*oldElement);

        if (deep) {
            for (Node* oldChild = oldElement->firstChild(); oldChild; oldChild = oldChild->nextSibling()) {
                RefPtr<Node> newChild = importNode(oldChild, true, es);
                if (es.hadException())
                    return 0;
                newElement->appendChild(newChild.release(), es);
                if (es.hadException())
                    return 0;
            }
        }

        return newElement.release();
    }
    case ATTRIBUTE_NODE:
        return Attr::create(*this, QualifiedName(nullAtom, toAttr(importedNode)->name(), nullAtom), toAttr(importedNode)->value());
    case DOCUMENT_FRAGMENT_NODE: {
        if (importedNode->isShadowRoot()) {
            break;
        }
        DocumentFragment* oldFragment = toDocumentFragment(importedNode);
        RefPtr<DocumentFragment> newFragment = createDocumentFragment();
        if (deep) {
            for (Node* oldChild = oldFragment->firstChild(); oldChild; oldChild = oldChild->nextSibling()) {
                RefPtr<Node> newChild = importNode(oldChild, true, es);
                if (es.hadException())
                    return 0;
                newFragment->appendChild(newChild.release(), es);
                if (es.hadException())
                    return 0;
            }
        }

        return newFragment.release();
    }
    case ENTITY_NODE:
    case NOTATION_NODE:
    case DOCUMENT_NODE:
    case DOCUMENT_TYPE_NODE:
    case XPATH_NAMESPACE_NODE:
        break;
    }
    es.throwUninformativeAndGenericDOMException(NotSupportedError);
    return 0;
}