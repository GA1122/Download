void Document::setTitleElement(Element* titleElement)
{
    if (m_titleElement && m_titleElement != titleElement) {
        if (isHTMLDocument() || isXHTMLDocument()) {
            m_titleElement = Traversal<HTMLTitleElement>::firstWithin(*this);
        } else if (isSVGDocument()) {
            m_titleElement = Traversal<SVGTitleElement>::firstWithin(*this);
        }
    } else {
        m_titleElement = titleElement;
    }

    if (isHTMLTitleElement(m_titleElement))
        updateTitle(toHTMLTitleElement(m_titleElement)->text());
    else if (isSVGTitleElement(m_titleElement))
        updateTitle(toSVGTitleElement(m_titleElement)->textContent());
}
