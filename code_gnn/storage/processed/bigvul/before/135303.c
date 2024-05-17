void Document::mediaQueryAffectingValueChanged()
{
    styleResolverChanged();
    m_evaluateMediaQueriesOnStyleRecalc = true;
    styleEngine().clearMediaQueryRuleSetStyleSheets();
    InspectorInstrumentation::mediaQueryResultChanged(this);
}
