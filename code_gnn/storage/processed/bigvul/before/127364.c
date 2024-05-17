static inline bool applyAuthorStylesOf(const Element* element)
{
    return element->treeScope().applyAuthorStyles() || (element->shadow() && element->shadow()->applyAuthorStyles());
}
