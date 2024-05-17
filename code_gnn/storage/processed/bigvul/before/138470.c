DEFINE_TRACE(HTMLScriptRunner)
{
    visitor->trace(m_document);
    visitor->trace(m_host);
    visitor->trace(m_parserBlockingScript);
#if ENABLE(OILPAN)
    visitor->trace(m_scriptsToExecuteAfterParsing);
#endif
}
