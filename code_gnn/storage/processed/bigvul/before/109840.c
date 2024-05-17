bool Document::shouldScheduleLayout()
{

    return (haveStylesheetsLoaded() && body())
        || (documentElement() && !isHTMLHtmlElement(documentElement()));
}
