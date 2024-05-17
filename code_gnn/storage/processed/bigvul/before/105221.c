static void trimFragment(DocumentFragment* fragment, Node* nodeBeforeContext, Node* nodeAfterContext)
{
    ExceptionCode ec = 0;
    RefPtr<Node> next;
    for (RefPtr<Node> node = fragment->firstChild(); node; node = next) {
        if (nodeBeforeContext->isDescendantOf(node.get())) {
            next = node->traverseNextNode();
            continue;
        }
        next = node->traverseNextSibling();
        ASSERT(!node->contains(nodeAfterContext));
        node->parentNode()->removeChild(node.get(), ec);
        if (nodeBeforeContext == node)
            break;
    }

    ASSERT(nodeAfterContext->parentNode());
    for (RefPtr<Node> node = nodeAfterContext; node; node = next) {
        next = node->traverseNextSibling();
        node->parentNode()->removeChild(node.get(), ec);
        ASSERT(!ec);
    }
}
