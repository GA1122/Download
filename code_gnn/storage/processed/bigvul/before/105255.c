void HTMLElement::mapLanguageAttributeToLocale(const Attribute& attribute, StylePropertySet* style)
{
    ASSERT((attribute.name() == langAttr || attribute.name().matches(XMLNames::langAttr)));
    if (!attribute.isEmpty()) {
        addPropertyToAttributeStyle(style, CSSPropertyWebkitLocale, quoteCSSString(attribute.value()));
    } else {
        addPropertyToAttributeStyle(style, CSSPropertyWebkitLocale, CSSValueAuto);
    }
}
