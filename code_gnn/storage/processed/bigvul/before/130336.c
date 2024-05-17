bool supportsInvalidationWithSelectorList(CSSSelector::PseudoType pseudo)
{
    return pseudo == CSSSelector::PseudoAny
        || pseudo == CSSSelector::PseudoCue
        || pseudo == CSSSelector::PseudoHost
        || pseudo == CSSSelector::PseudoHostContext
        || pseudo == CSSSelector::PseudoNot;
}
