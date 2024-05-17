void Document::documentWillBecomeInactive()
{
#if USE(ACCELERATED_COMPOSITING)
    if (renderer())
        renderView()->setIsInWindow(false);
#endif
}
