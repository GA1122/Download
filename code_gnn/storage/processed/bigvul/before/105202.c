 static inline bool hasOneChild(ContainerNode* node)
 {
     Node* firstChild = node->firstChild();
    return firstChild && !firstChild->nextSibling();
}
