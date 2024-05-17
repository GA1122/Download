static inline bool elementAffectsDirectionality(const Node* node)
{
    return node->isHTMLElement() && (node->hasTagName(bdiTag) || toHTMLElement(node)->hasAttribute(dirAttr));
}
