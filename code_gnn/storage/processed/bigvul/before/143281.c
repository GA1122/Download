PassRefPtr<ComputedStyle> Document::styleForElementIgnoringPendingStylesheets(Element* element)
{
    DCHECK_EQ(element->document(), this);
    StyleEngine::IgnoringPendingStylesheet ignoring(styleEngine());
    return ensureStyleResolver().styleForElement(element, element->parentNode() ? element->parentNode()->ensureComputedStyle() : 0);
}
