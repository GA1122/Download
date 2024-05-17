 bool HTMLDocumentParser::hasInsertionPoint()
 {
    return m_input.hasInsertionPoint() || (wasCreatedByScript() && !m_input.haveSeenEndOfFile());
}
