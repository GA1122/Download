PassRefPtr<RenderStyle> Document::styleForElementIgnoringPendingStylesheets(Element* element)
{
    ASSERT_ARG(element, element->document() == this);
    TemporaryChange<bool> ignoreStyleSheets(m_ignorePendingStylesheets, true);
    return styleResolver()->styleForElement(element, element->parentNode() ? element->parentNode()->computedStyle() : 0);
}
