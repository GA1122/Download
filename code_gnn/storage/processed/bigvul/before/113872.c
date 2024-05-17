HTMLDocumentParser::HTMLDocumentParser(HTMLDocument* document, bool reportErrors)
    : ScriptableDocumentParser(document)
    , m_options(document)
    , m_tokenizer(HTMLTokenizer::create(m_options))
    , m_scriptRunner(HTMLScriptRunner::create(document, this))
    , m_treeBuilder(HTMLTreeBuilder::create(this, document, reportErrors, m_options))
    , m_parserScheduler(HTMLParserScheduler::create(this))
    , m_xssAuditor(this)
    , m_endWasDelayed(false)
    , m_pumpSessionNestingLevel(0)
{
}
