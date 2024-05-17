void HTMLLinkElement::tokenizeRelAttribute(const AtomicString& rel, RelAttribute& relAttribute)
{
    relAttribute.m_isStyleSheet = false;
    relAttribute.m_isIcon = false;
    relAttribute.m_isAlternate = false;
    relAttribute.m_isDNSPrefetch = false;
#if ENABLE(LINK_PREFETCH)
    relAttribute.m_isLinkPrefetch = false;
#endif
    if (equalIgnoringCase(rel, "stylesheet"))
        relAttribute.m_isStyleSheet = true;
    else if (equalIgnoringCase(rel, "icon") || equalIgnoringCase(rel, "shortcut icon"))
        relAttribute.m_isIcon = true;
    else if (equalIgnoringCase(rel, "dns-prefetch"))
        relAttribute.m_isDNSPrefetch = true;
#if ENABLE(LINK_PREFETCH)
    else if (equalIgnoringCase(rel, "prefetch"))
        relAttribute.m_isLinkPrefetch = true;
#endif
    else if (equalIgnoringCase(rel, "alternate stylesheet") || equalIgnoringCase(rel, "stylesheet alternate")) {
        relAttribute.m_isStyleSheet = true;
        relAttribute.m_isAlternate = true;
    } else {
        String relString = rel.string();
        relString.replace('\n', ' ');
        Vector<String> list;
        relString.split(' ', list);
        Vector<String>::const_iterator end = list.end();
        for (Vector<String>::const_iterator it = list.begin(); it != end; ++it) {
            if (equalIgnoringCase(*it, "stylesheet"))
                relAttribute.m_isStyleSheet = true;
            else if (equalIgnoringCase(*it, "alternate"))
                relAttribute.m_isAlternate = true;
            else if (equalIgnoringCase(*it, "icon"))
                relAttribute.m_isIcon = true;
        }
    }
}
