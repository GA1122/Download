Node* WebPagePrivate::bestChildNodeForClickRect(Node* parentNode, const IntRect& clickRect)
{
    if (!parentNode)
        return 0;

    int bestDistance = std::numeric_limits<int>::max();

    Node* node = parentNode->firstChild();
    Node* bestNode = 0;
    for (; node; node = node->nextSibling()) {
        IntRect rect = rectForNode(node);
        if (!clickRect.intersects(rect))
            continue;

        int distance = distanceBetweenPoints(rect.center(), clickRect.center());
        Node* bestChildNode = bestChildNodeForClickRect(node, clickRect);
        if (bestChildNode) {
            IntRect bestChildRect = rectForNode(bestChildNode);
            int bestChildDistance = distanceBetweenPoints(bestChildRect.center(), clickRect.center());
            if (bestChildDistance < distance && bestChildDistance < bestDistance) {
                bestNode = bestChildNode;
                bestDistance = bestChildDistance;
            } else {
                if (distance < bestDistance) {
                    bestNode = node;
                    bestDistance = distance;
                }
            }
        } else {
            if (distance < bestDistance) {
                bestNode = node;
                bestDistance = distance;
            }
        }
    }

    return bestNode;
}
