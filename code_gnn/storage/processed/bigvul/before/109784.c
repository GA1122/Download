void Document::removeTitle(Element* titleElement)
{
    if (m_titleElement != titleElement)
        return;

    m_titleElement = 0;
    m_titleSetExplicitly = false;

    if (HTMLElement* headElement = head()) {
        for (Element* element = headElement->firstElementChild(); element; element = element->nextElementSibling()) {
            if (!isHTMLTitleElement(element))
                continue;
            HTMLTitleElement* title = toHTMLTitleElement(element);
            setTitleElement(title->text(), title);
            break;
        }
    }

    if (!m_titleElement)
        updateTitle(String());
}
