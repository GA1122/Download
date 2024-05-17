void Document::mediaQueryAffectingValueChanged()
{
    m_evaluateMediaQueriesOnStyleRecalc = true;
    styleEngine().clearMediaQueryRuleSetStyleSheets();
    InspectorInstrumentation::mediaQueryResultChanged(this);
}
