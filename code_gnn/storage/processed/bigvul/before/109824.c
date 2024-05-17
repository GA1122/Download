void Document::setTitle(const String& title)
{
    m_titleSetExplicitly = true;
    if (!isHTMLDocument() && !isXHTMLDocument())
        m_titleElement = 0;
    else if (!m_titleElement) {
        if (HTMLElement* headElement = head()) {
            m_titleElement = createElement(titleTag, false);
            headElement->appendChild(m_titleElement);
        }
    }

    updateTitle(title);

    if (m_titleElement && isHTMLTitleElement(m_titleElement.get()))
        toHTMLTitleElement(m_titleElement.get())->setText(title);
}
