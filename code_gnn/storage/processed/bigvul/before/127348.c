RuleSet* CSSDefaultStyleSheets::xhtmlMobileProfileStyle()
{
    if (!defaultXHTMLMobileProfileStyle) {
        defaultXHTMLMobileProfileStyle = RuleSet::create().leakPtr();
        defaultXHTMLMobileProfileStyle->addRulesFromSheet(parseUASheet(xhtmlmpUserAgentStyleSheet, sizeof(xhtmlmpUserAgentStyleSheet)), screenEval());
    }
    return defaultXHTMLMobileProfileStyle;
}
