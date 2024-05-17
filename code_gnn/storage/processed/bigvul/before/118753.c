static inline bool checkAddChild(ContainerNode* newParent, Node* newChild, ExceptionState& es)
{
    return checkAcceptChild(newParent, newChild, 0, es);
}
