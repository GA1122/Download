void Document::removeTitle(Element* titleElement)
{
    if (m_titleElement != titleElement)
        return;

    m_titleElement = nullptr;

    if (isHTMLDocument() || isXHTMLDocument()) {
        if (HTMLTitleElement* title = Traversal<HTMLTitleElement>::firstWithin(*this))
            setTitleElement(title);
    } else if (isSVGDocument()) {
        if (SVGTitleElement* title = Traversal<SVGTitleElement>::firstWithin(*this))
            setTitleElement(title);
    }

    if (!m_titleElement)
        updateTitle(String());
}
