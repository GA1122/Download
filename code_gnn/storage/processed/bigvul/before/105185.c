static inline Node* ancestorToRetainStructureAndAppearanceWithNoRenderer(Node* commonAncestor)
{
    Node* commonAncestorBlock = enclosingNodeOfType(firstPositionInOrBeforeNode(commonAncestor), isHTMLBlockElement);
    return ancestorToRetainStructureAndAppearanceForBlock(commonAncestorBlock);
}
