void HTMLScriptRunner::executeParsingBlockingScripts()
{
    while (hasParserBlockingScript() && isPendingScriptReady(m_parserBlockingScript))
        executeParsingBlockingScript();
}
