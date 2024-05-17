static bool findNodesSurroundingContext(Document* document, RefPtr<Node>& nodeBeforeContext, RefPtr<Node>& nodeAfterContext)
{
    for (Node* node = document->firstChild(); node; node = node->traverseNextNode()) {
        if (node->nodeType() == Node::COMMENT_NODE && static_cast<CharacterData*>(node)->data() == fragmentMarkerTag) {
            if (!nodeBeforeContext)
                nodeBeforeContext = node;
            else {
                nodeAfterContext = node;
                return true;
            }
        }
    }
    return false;
}
