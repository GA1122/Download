PassRefPtr<CSSStyleDeclaration> Document::createCSSStyleDeclaration()
{
    return StylePropertySet::create()->ensureCSSStyleDeclaration();
}
