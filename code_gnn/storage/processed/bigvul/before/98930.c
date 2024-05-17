void HTMLConstructionSite::insertHTMLFormElement(AtomicHTMLToken& token)
{
    insertHTMLElement(token);
    ASSERT(currentElement()->isHTMLElement());
    ASSERT(currentElement()->hasTagName(formTag));
    m_form = static_cast<HTMLFormElement*>(currentElement());
}
