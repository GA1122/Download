void Document::setTitleElement(Element* titleElement)
{
    if (isSVGSVGElement(documentElement())) {
        m_titleElement = Traversal<SVGTitleElement>::firstChild(*documentElement());
    } else {
        if (m_titleElement && m_titleElement != titleElement)
            m_titleElement = Traversal<HTMLTitleElement>::firstWithin(*this);
        else
            m_titleElement = titleElement;

        if (isSVGTitleElement(m_titleElement)) {
            m_titleElement = nullptr;
            return;
        }
    }

    if (isHTMLTitleElement(m_titleElement))
        updateTitle(toHTMLTitleElement(m_titleElement)->text());
    else if (isSVGTitleElement(m_titleElement))
        updateTitle(toSVGTitleElement(m_titleElement)->textContent());
}
