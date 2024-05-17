static inline unsigned lengthOfContentsInNode(Node* node)
{
    switch (node->nodeType()) {
    case Node::TEXT_NODE:
    case Node::CDATA_SECTION_NODE:
    case Node::COMMENT_NODE:
        return static_cast<CharacterData*>(node)->length();
    case Node::PROCESSING_INSTRUCTION_NODE:
        return static_cast<ProcessingInstruction*>(node)->data().length();
    case Node::ELEMENT_NODE:
    case Node::ATTRIBUTE_NODE:
    case Node::ENTITY_REFERENCE_NODE:
    case Node::ENTITY_NODE:
    case Node::DOCUMENT_NODE:
    case Node::DOCUMENT_TYPE_NODE:
    case Node::DOCUMENT_FRAGMENT_NODE:
    case Node::NOTATION_NODE:
    case Node::XPATH_NAMESPACE_NODE:
        return node->childNodeCount();
    }
    ASSERT_NOT_REACHED();
    return 0;
}
