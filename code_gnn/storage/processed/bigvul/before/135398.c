PassRefPtr<ComputedStyle> Document::styleForElementIgnoringPendingStylesheets(Element* element)
{
    ASSERT_ARG(element, element->document() == this);
    StyleEngine::IgnoringPendingStylesheet ignoring(styleEngine());
    return ensureStyleResolver().styleForElement(element, element->parentNode() ? element->parentNode()->ensureComputedStyle() : 0);
}
