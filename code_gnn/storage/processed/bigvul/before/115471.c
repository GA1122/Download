static WTF::String styleDecToStr(WKBundleCSSStyleDeclarationRef style)
{
    StringBuilder stringBuilder;
    stringBuilder.appendLiteral("<DOMCSSStyleDeclaration ADDRESS>");
    return stringBuilder.toString();
}
