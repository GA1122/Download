bool Document::shouldParserYieldAgressivelyBeforeScriptExecution()
{
    return view() && view()->layoutPending() && !minimumLayoutDelay();
}
