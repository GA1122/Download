void InspectorCSSOMWrappers::collectFromStyleEngine(StyleEngine* styleSheetCollection)
{
    Vector<const Vector<RefPtr<CSSStyleSheet> >*> activeAuthorStyleSheets;
    styleSheetCollection->getActiveAuthorStyleSheets(activeAuthorStyleSheets);
    for (size_t i = 0; i < activeAuthorStyleSheets.size(); ++i)
        collectFromStyleSheets(*activeAuthorStyleSheets[i]);
}
