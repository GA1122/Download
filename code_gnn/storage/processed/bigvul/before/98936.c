void HTMLConstructionSite::insertSelfClosingHTMLElement(AtomicHTMLToken& token)
{
    ASSERT(token.type() == HTMLToken::StartTag);
    RefPtr<Element> element = attachToCurrent(createHTMLElement(token));
    element->finishParsingChildren();
}
