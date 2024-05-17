void Document::setTitle(const String& title)
{
    m_titleSetExplicitly = true;
    if (!isHTMLDocument() && !isXHTMLDocument())
        m_titleElement = 0;
    else if (!m_titleElement) {
        if (HTMLElement* headElement = head()) {
            m_titleElement = createElement(titleTag, false);
            headElement->appendChild(m_titleElement, ASSERT_NO_EXCEPTION);
        }
    }

    updateTitle(StringWithDirection(title, LTR));

    if (m_titleElement) {
        ASSERT(m_titleElement->hasTagName(titleTag));
        if (m_titleElement->hasTagName(titleTag))
            static_cast<HTMLTitleElement*>(m_titleElement.get())->setText(title);
    }
}
