bool requiresSubtreeInvalidation(const CSSSelector& selector)
{
    if (selector.match() != CSSSelector::PseudoElement && selector.match() != CSSSelector::PseudoClass) {
        ASSERT(supportsInvalidation(selector.match()));
        return false;
    }

    switch (selector.pseudoType()) {
    case CSSSelector::PseudoFirstLine:
    case CSSSelector::PseudoFirstLetter:
    case CSSSelector::PseudoHostContext:
        return true;
    default:
        ASSERT(supportsInvalidation(selector.pseudoType()));
        return false;
    }
}
