void HTMLDocumentParser::runScriptsForPausedTreeBuilder()
{
    TextPosition scriptStartPosition = TextPosition::belowRangePosition();
    RefPtr<Element> scriptElement = m_treeBuilder->takeScriptToProcess(scriptStartPosition);
    if (m_scriptRunner)
        m_scriptRunner->execute(scriptElement.release(), scriptStartPosition);
}
