void HTMLLinkElement::addSubresourceAttributeURLs(ListHashSet<KURL>& urls) const
{
    HTMLElement::addSubresourceAttributeURLs(urls);

    if (m_relAttribute.m_isIcon)
        return;

    if (!m_relAttribute.m_isStyleSheet)
        return;
    
    addSubresourceURL(urls, href());
    
    if (StyleSheet* styleSheet = const_cast<HTMLLinkElement*>(this)->sheet())
        styleSheet->addSubresourceStyleURLs(urls);
}
