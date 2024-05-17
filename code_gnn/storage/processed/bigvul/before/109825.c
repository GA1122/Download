void Document::setTitleElement(const String& title, Element* titleElement)
{
    if (titleElement != m_titleElement) {
        if (m_titleElement || m_titleSetExplicitly)
            return;
        m_titleElement = titleElement;
    }

    updateTitle(title);
}
