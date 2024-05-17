void InspectorCSSOMWrappers::collect(ListType* listType)
{
    if (!listType)
        return;
    unsigned size = listType->length();
    for (unsigned i = 0; i < size; ++i) {
        CSSRule* cssRule = listType->item(i);
        switch (cssRule->type()) {
        case CSSRule::IMPORT_RULE:
            collect(static_cast<CSSImportRule*>(cssRule)->styleSheet());
            break;
        case CSSRule::MEDIA_RULE:
            collect(static_cast<CSSMediaRule*>(cssRule));
            break;
        case CSSRule::SUPPORTS_RULE:
            collect(static_cast<CSSSupportsRule*>(cssRule));
            break;
        case CSSRule::WEBKIT_REGION_RULE:
            collect(static_cast<CSSRegionRule*>(cssRule));
            break;
        case CSSRule::HOST_RULE:
            collect(static_cast<CSSHostRule*>(cssRule));
            break;
        case CSSRule::STYLE_RULE:
            m_styleRuleToCSSOMWrapperMap.add(static_cast<CSSStyleRule*>(cssRule)->styleRule(), static_cast<CSSStyleRule*>(cssRule));
            break;
        default:
            break;
        }
    }
}
