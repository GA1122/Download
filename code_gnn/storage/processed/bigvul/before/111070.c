Node* WebPagePrivate::bestNodeForZoomUnderPoint(const IntPoint& documentPoint)
{
    IntRect clickRect(documentPoint.x() - blockClickRadius, documentPoint.y() - blockClickRadius, 2 * blockClickRadius, 2 * blockClickRadius);
    Node* originalNode = nodeForZoomUnderPoint(documentPoint);
    if (!originalNode)
        return 0;
    Node* node = bestChildNodeForClickRect(originalNode, clickRect);
    return node ? adjustedBlockZoomNodeForZoomAndExpandingRatioLimits(node) : adjustedBlockZoomNodeForZoomAndExpandingRatioLimits(originalNode);
}
