static inline bool frameElementAndViewPermitScroll(HTMLFrameElementBase* frameElementBase, FrameView* frameView)
{
    if (frameElementBase && frameElementBase->scrollingMode() != ScrollbarAlwaysOff)
        return true;

    if (frameView->wasScrolledByUser())
        return false;

    Page* page = frameView->frame().page();
    if (!page)
        return false;
    return !page->autoscrollController().autoscrollInProgress();
}
