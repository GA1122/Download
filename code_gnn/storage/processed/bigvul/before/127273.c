void PageSerializer::retrieveResourcesForCSSValue(CSSValue* cssValue, Document* document)
{
    if (cssValue->isImageValue()) {
        CSSImageValue* imageValue = toCSSImageValue(cssValue);
        StyleImage* styleImage = imageValue->cachedOrPendingImage();
        if (!styleImage || !styleImage->isImageResource())
            return;

        addImageToResources(styleImage->cachedImage(), 0, styleImage->cachedImage()->url());
    } else if (cssValue->isFontFaceSrcValue()) {
        CSSFontFaceSrcValue* fontFaceSrcValue = toCSSFontFaceSrcValue(cssValue);
        if (fontFaceSrcValue->isLocal()) {
            return;
        }

        addFontToResources(fontFaceSrcValue->fetch(document));
    } else if (cssValue->isValueList()) {
        CSSValueList* cssValueList = toCSSValueList(cssValue);
        for (unsigned i = 0; i < cssValueList->length(); i++)
            retrieveResourcesForCSSValue(cssValueList->item(i), document);
    }
}
