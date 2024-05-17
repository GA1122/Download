HTMLDocumentParser::HTMLDocumentParser(DocumentFragment* fragment, Element* contextElement, FragmentScriptingPermission scriptingPermission)
    : ScriptableDocumentParser(fragment->document())
    , m_options(fragment->document())
    , m_tokenizer(HTMLTokenizer::create(m_options))
    , m_treeBuilder(HTMLTreeBuilder::create(this, fragment, contextElement, scriptingPermission, m_options))
    , m_xssAuditor(this)
    , m_endWasDelayed(false)
    , m_pumpSessionNestingLevel(0)
{
    bool reportErrors = false;  
    m_tokenizer->setState(tokenizerStateForContextElement(contextElement, reportErrors, m_options));
}
