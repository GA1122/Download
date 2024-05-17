 RuleSet* CSSDefaultStyleSheets::viewSourceStyle()
 {
     if (!defaultViewSourceStyle) {
        defaultViewSourceStyle = RuleSet::create().leakPtr();
        defaultViewSourceStyle->addRulesFromSheet(parseUASheet(sourceUserAgentStyleSheet, sizeof(sourceUserAgentStyleSheet)), screenEval());
    }
    return defaultViewSourceStyle;
}
