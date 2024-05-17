void FrameView::removeViewportConstrainedObject(RenderObject* object)
{
    if (m_viewportConstrainedObjects && m_viewportConstrainedObjects->contains(object)) {
        m_viewportConstrainedObjects->remove(object);

        if (Page* page = m_frame->page()) {
            if (ScrollingCoordinator* scrollingCoordinator = page->scrollingCoordinator())
                scrollingCoordinator->frameViewFixedObjectsDidChange(this);
        }
    }
}
