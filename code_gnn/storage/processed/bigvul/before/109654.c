void Document::documentWillBecomeInactive()
{
    if (renderer())
        renderView()->setIsInWindow(false);
}
