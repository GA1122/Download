void HTMLDocumentParser::constructTreeFromHTMLToken(HTMLToken& rawToken)
{
    RefPtr<AtomicHTMLToken> token = AtomicHTMLToken::create(rawToken);

    if (rawToken.type() != HTMLTokenTypes::Character)
        rawToken.clear();

    m_treeBuilder->constructTree(token.get());

    token->clearExternalCharacters();

    if (!rawToken.isUninitialized()) {
        ASSERT(rawToken.type() == HTMLTokenTypes::Character);
        rawToken.clear();
     }
 }
