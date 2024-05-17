Node* WebPagePrivate::adjustedBlockZoomNodeForZoomAndExpandingRatioLimits(Node* node)
{
    Node* initialNode = node;
    RenderObject* renderer = node->renderer();
    bool acceptableNodeSize = newScaleForBlockZoomRect(rectForNode(node), 1.0, 0) < maxBlockZoomScale();
    IntSize actualVisibleSize = this->actualVisibleSize();

    while (!renderer || !acceptableNodeSize) {
        node = node->parentNode();
        IntRect nodeRect = rectForNode(node);

        if (!node || static_cast<double>(actualVisibleSize.width() - nodeRect.width()) / actualVisibleSize.width() < minimumExpandingRatio)
            return initialNode;

        renderer = node->renderer();
        acceptableNodeSize = newScaleForBlockZoomRect(rectForNode(node), 1.0, 0) < maxBlockZoomScale();
    }

    return node;
}
