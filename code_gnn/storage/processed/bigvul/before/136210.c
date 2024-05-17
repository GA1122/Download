void PageSerializer::retrieveResourcesForProperties(const StylePropertySet* styleDeclaration, Document& document)
{
    if (!styleDeclaration)
        return;

    unsigned propertyCount = styleDeclaration->propertyCount();
    for (unsigned i = 0; i < propertyCount; ++i) {
        RefPtrWillBeRawPtr<CSSValue> cssValue = styleDeclaration->propertyAt(i).value();
        retrieveResourcesForCSSValue(cssValue.get(), document);
    }
}
