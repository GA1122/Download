IntRect WebPagePrivate::blockZoomRectForNode(Node* node)
{
    if (!node || contentsSize().isEmpty())
        return IntRect();

    Node* tnode = node;
    m_currentBlockZoomAdjustedNode = tnode;

    IntRect blockRect = rectForNode(tnode);
    IntRect originalRect = blockRect;

    int originalArea = originalRect.width() * originalRect.height();
    int pageArea = contentsSize().width() * contentsSize().height();
    double blockToPageRatio = static_cast<double>(pageArea - originalArea) / pageArea;
    double blockExpansionRatio = 5.0 * blockToPageRatio * blockToPageRatio;

    if (!tnode->hasTagName(HTMLNames::imgTag) && !tnode->hasTagName(HTMLNames::inputTag) && !tnode->hasTagName(HTMLNames::textareaTag)) {
        while (tnode = tnode->parentNode()) {
            ASSERT(tnode);
            IntRect tRect = rectForNode(tnode);
            int tempBlockArea = tRect.width() * tRect.height();
            if (static_cast<double>(pageArea - tempBlockArea) / pageArea < minimumExpandingRatio)
                break;
            if (tRect.isEmpty())
                continue;  
            if (tempBlockArea < 1.1 * originalArea)
                continue;  
            IntSize actualSize = actualVisibleSize();
            if (static_cast<double>(actualSize.width() - tRect.width()) / actualSize.width() < minimumExpandingRatio)
                break;
            if (tempBlockArea < blockExpansionRatio * originalArea) {
                blockRect = tRect;
                m_currentBlockZoomAdjustedNode = tnode;
            } else
                break;
        }
    }

    blockRect = adjustRectOffsetForFrameOffset(blockRect, node);
    blockRect = mapToTransformed(blockRect);
    clipToTransformedContentsRect(blockRect);

    return blockRect;
}
