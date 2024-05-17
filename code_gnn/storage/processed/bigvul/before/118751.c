static inline bool checkAcceptChild(ContainerNode* newParent, Node* newChild, Node* oldChild, ExceptionState& es)
{
    if (!newChild) {
        es.throwUninformativeAndGenericDOMException(NotFoundError);
        return false;
    }

    if ((newChild->isElementNode() || newChild->isTextNode()) && newParent->isElementNode()) {
        ASSERT(!newParent->isDocumentTypeNode());
        ASSERT(isChildTypeAllowed(newParent, newChild));
        if (containsConsideringHostElements(newChild, newParent)) {
            es.throwUninformativeAndGenericDOMException(HierarchyRequestError);
            return false;
        }
        return true;
    }

    ASSERT(!newChild->isPseudoElement());
    if (newChild->isPseudoElement()) {
        es.throwUninformativeAndGenericDOMException(HierarchyRequestError);
        return false;
    }

    if (containsConsideringHostElements(newChild, newParent)) {
        es.throwUninformativeAndGenericDOMException(HierarchyRequestError);
        return false;
    }

    if (oldChild && newParent->isDocumentNode()) {
        if (!toDocument(newParent)->canReplaceChild(newChild, oldChild)) {
            es.throwUninformativeAndGenericDOMException(HierarchyRequestError);
            return false;
        }
    } else if (!isChildTypeAllowed(newParent, newChild)) {
        es.throwUninformativeAndGenericDOMException(HierarchyRequestError);
        return false;
    }

    return true;
}
