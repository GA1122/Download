PassRefPtr<RenderStyle> Document::styleForElementIgnoringPendingStylesheets(Element* element)
{
    ASSERT_ARG(element, element->document() == this);

    bool oldIgnore = m_ignorePendingStylesheets;
    m_ignorePendingStylesheets = true;
    RefPtr<RenderStyle> style = styleResolver()->styleForElement(element, element->parentNode() ? element->parentNode()->computedStyle() : 0);
    m_ignorePendingStylesheets = oldIgnore;
    return style.release();
}
