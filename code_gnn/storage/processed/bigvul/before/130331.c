bool RuleFeatureSet::extractInvalidationSetFeature(const CSSSelector& selector, InvalidationSetFeatures& features)
{
    if (selector.match() == CSSSelector::Tag && selector.tagQName().localName() != starAtom)
        features.tagName = selector.tagQName().localName();
    else if (selector.match() == CSSSelector::Id)
        features.id = selector.value();
    else if (selector.match() == CSSSelector::Class)
        features.classes.append(selector.value());
    else if (selector.isAttributeSelector())
        features.attributes.append(selector.attribute().localName());
    else if (selector.pseudoType() == CSSSelector::PseudoWebKitCustomElement)
        features.customPseudoElement = true;
    else if (selector.pseudoType() == CSSSelector::PseudoBefore || selector.pseudoType() == CSSSelector::PseudoAfter)
        features.hasBeforeOrAfter = true;
    else
        return false;
    return true;
}
