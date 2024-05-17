static void wheelEventHandlerCountChanged(Document* document)
{
    Page* page = document->page();
    if (!page)
        return;

    ScrollingCoordinator* scrollingCoordinator = page->scrollingCoordinator();
    if (!scrollingCoordinator)
        return;

    FrameView* frameView = document->view();
    if (!frameView)
        return;

    scrollingCoordinator->frameViewWheelEventHandlerCountChanged(frameView);
}
