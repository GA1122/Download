void InspectorCSSOMWrappers::collectFromStyleSheets(const Vector<RefPtr<CSSStyleSheet> >& sheets)
{
    for (unsigned i = 0; i < sheets.size(); ++i)
        collect(sheets[i].get());
}
