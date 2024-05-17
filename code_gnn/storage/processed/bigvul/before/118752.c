static inline bool checkAcceptChildGuaranteedNodeTypes(ContainerNode* newParent, Node* newChild, ExceptionState& es)
{
    ASSERT(!newParent->isDocumentTypeNode());
    ASSERT(isChildTypeAllowed(newParent, newChild));
    if (newChild->contains(newParent)) {
        es.throwUninformativeAndGenericDOMException(HierarchyRequestError);
        return false;
    }

    return true;
}
