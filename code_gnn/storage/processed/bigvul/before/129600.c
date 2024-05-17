static bool hasValidPredecessor(const Node* node)
{
    ASSERT(node);
    for (node = node->previousSibling(); node; node = node->previousSibling()) {
        if (node->isSVGElement() && toSVGElement(node)->isValid())
            return true;
    }
    return false;
}
