void Document::setActiveElement(PassRefPtr<Element> newActiveElement)
{
    if (!newActiveElement) {
        m_activeElement.clear();
        return;
    }

    m_activeElement = newActiveElement;
}
