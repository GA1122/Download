void StyledMarkupAccumulator::wrapWithNode(Node* node, bool convertBlocksToInlines, RangeFullySelectsNode rangeFullySelectsNode)
{
    StringBuilder markup;
    if (node->isElementNode())
        appendElement(markup, static_cast<Element*>(node), convertBlocksToInlines && isBlock(const_cast<Node*>(node)), rangeFullySelectsNode);
    else
        appendStartMarkup(markup, node, 0);
    m_reversedPrecedingMarkup.append(markup.toString());
    appendEndTag(node);
    if (m_nodes)
        m_nodes->append(node);
}
