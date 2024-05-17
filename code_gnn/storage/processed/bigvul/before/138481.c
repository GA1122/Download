bool HTMLScriptRunner::hasParserBlockingScript() const
{
    return !!m_parserBlockingScript.element();
}
