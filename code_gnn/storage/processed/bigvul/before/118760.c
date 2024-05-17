static inline bool containsConsideringHostElements(const Node* newChild, const Node* newParent)
{
    return (newParent->isInShadowTree() || isInTemplateContent(newParent))
        ? newChild->containsIncludingHostElements(newParent)
        : newChild->contains(newParent);
}
