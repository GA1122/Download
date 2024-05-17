bool HTMLDocumentParser::isWaitingForScripts() const
{
    bool treeBuilderHasBlockingScript = m_treeBuilder->hasParserBlockingScript();
    bool scriptRunnerHasBlockingScript = m_scriptRunner && m_scriptRunner->hasParserBlockingScript();
    ASSERT(!(treeBuilderHasBlockingScript && scriptRunnerHasBlockingScript));
    return treeBuilderHasBlockingScript || scriptRunnerHasBlockingScript;
}
