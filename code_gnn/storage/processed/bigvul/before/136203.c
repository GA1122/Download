void SerializerMarkupAccumulator::appendStartTag(Node& node, Namespaces* namespaces)
{
    MarkupAccumulator::appendStartTag(node, namespaces);
    m_nodes.append(&node);
}
