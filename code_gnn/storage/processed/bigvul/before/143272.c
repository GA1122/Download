void Document::setTitle(const String& title)
{
    if (!m_titleElement) {
        if (isHTMLDocument() || isXHTMLDocument()) {
            HTMLElement* headElement = head();
            if (!headElement)
                return;
            m_titleElement = HTMLTitleElement::create(*this);
            headElement->appendChild(m_titleElement.get());
        } else if (isSVGDocument()) {
            Element* element = documentElement();
            if (!isSVGSVGElement(element))
                return;
            m_titleElement = SVGTitleElement::create(*this);
            element->insertBefore(m_titleElement.get(), element->firstChild());
        }
    } else {
        if (!isHTMLDocument() && !isXHTMLDocument() && !isSVGDocument())
            m_titleElement = nullptr;
    }

    if (isHTMLTitleElement(m_titleElement))
        toHTMLTitleElement(m_titleElement)->setText(title);
    else if (isSVGTitleElement(m_titleElement))
        toSVGTitleElement(m_titleElement)->setText(title);
    else
        updateTitle(title);
}
