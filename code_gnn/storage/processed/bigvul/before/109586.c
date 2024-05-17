PassRefPtr<Range> Document::caretRangeFromPoint(int x, int y)
{
    if (!renderer())
        return 0;
    LayoutPoint localPoint;
    RenderObject* renderer = rendererFromPoint(this, x, y, &localPoint);
    if (!renderer)
        return 0;

    Node* node = renderer->node();
    Node* shadowAncestorNode = ancestorInThisScope(node);
    if (shadowAncestorNode != node) {
        unsigned offset = shadowAncestorNode->nodeIndex();
        ContainerNode* container = shadowAncestorNode->parentNode();
        return Range::create(*this, container, offset, container, offset);
    }

    PositionWithAffinity positionWithAffinity = renderer->positionForPoint(localPoint);
    if (positionWithAffinity.position().isNull())
        return 0;

    Position rangeCompliantPosition = positionWithAffinity.position().parentAnchoredEquivalent();
    return Range::create(*this, rangeCompliantPosition, rangeCompliantPosition);
}
