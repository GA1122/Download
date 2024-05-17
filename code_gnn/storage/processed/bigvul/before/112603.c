void Document::updateBaseURL()
{
    KURL oldBaseURL = m_baseURL;
    if (!m_baseElementURL.isEmpty())
        m_baseURL = m_baseElementURL;
    else if (!m_baseURLOverride.isEmpty())
        m_baseURL = m_baseURLOverride;
    else {
        m_baseURL = KURL(ParsedURLString, documentURI());
    }
    selectorQueryCache()->invalidate();

    if (!m_baseURL.isValid())
        m_baseURL = KURL();

    if (m_elemSheet) {
        ASSERT(!m_elemSheet->contents()->ruleCount());
        bool usesRemUnits = m_elemSheet->contents()->usesRemUnits();
        m_elemSheet = CSSStyleSheet::createInline(this, m_baseURL);
        m_elemSheet->contents()->parserSetUsesRemUnits(usesRemUnits);
    }

    if (!equalIgnoringFragmentIdentifier(oldBaseURL, m_baseURL)) {
        for (Element* element = ElementTraversal::firstWithin(this); element; element = ElementTraversal::next(element)) {
            if (element->hasTagName(aTag))
                static_cast<HTMLAnchorElement*>(element)->invalidateCachedVisitedLinkHash();
        }
    }
}
