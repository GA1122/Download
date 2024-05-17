void Document::setDir(const AtomicString& value)
{
    Element* rootElement = documentElement();
    if (isHTMLHtmlElement(rootElement))
        toHTMLHtmlElement(rootElement)->setDir(value);
}
