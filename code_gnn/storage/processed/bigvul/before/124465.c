SimplifyMarkupCommand::SimplifyMarkupCommand(Document& document, Node* firstNode, Node* nodeAfterLast)
    : CompositeEditCommand(document), m_firstNode(firstNode), m_nodeAfterLast(nodeAfterLast)
{
}
