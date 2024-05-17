String Range::toString(ExceptionCode& ec) const
{
    if (!m_start.container()) {
        ec = INVALID_STATE_ERR;
        return String();
    }

    StringBuilder builder;

    Node* pastLast = pastLastNode();
    for (Node* n = firstNode(); n != pastLast; n = n->traverseNextNode()) {
        if (n->nodeType() == Node::TEXT_NODE || n->nodeType() == Node::CDATA_SECTION_NODE) {
            String data = static_cast<CharacterData*>(n)->data();
            int length = data.length();
            int start = (n == m_start.container()) ? min(max(0, m_start.offset()), length) : 0;
            int end = (n == m_end.container()) ? min(max(start, m_end.offset()), length) : length;
            builder.append(data.characters() + start, end - start);
        }
    }

    return builder.toString();
}
