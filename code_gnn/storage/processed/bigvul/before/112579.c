bool Document::shouldScheduleLayout()
{

    return (haveStylesheetsLoaded() && body())
        || (documentElement() && !documentElement()->hasTagName(htmlTag));
}
