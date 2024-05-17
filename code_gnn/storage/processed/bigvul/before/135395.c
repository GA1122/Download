bool Document::shouldScheduleLayout() const
{
    if (!isActive())
        return false;

    if (isRenderingReady() && body())
        return true;

    if (documentElement() && !isHTMLHtmlElement(*documentElement()))
        return true;

    return false;
}
