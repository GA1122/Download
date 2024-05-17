void Document::setTitle(const String& title)
{
    if (!isHTMLDocument() && !isXHTMLDocument()) {
        m_titleElement = nullptr;
    } else if (!m_titleElement) {
        HTMLElement* headElement = head();
        if (!headElement)
            return;
        m_titleElement = HTMLTitleElement::create(*this);
        headElement->appendChild(m_titleElement.get());
    }

    if (isHTMLTitleElement(m_titleElement))
        toHTMLTitleElement(m_titleElement)->setText(title);
    else
        updateTitle(title);
}
