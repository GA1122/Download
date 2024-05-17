bool Document::hasFocus() const
{
    Page* page = this->page();
    if (!page)
        return false;
    if (!page->focusController().isActive() || !page->focusController().isFocused())
        return false;
    Frame* focusedFrame = page->focusController().focusedFrame();
    if (focusedFrame && focusedFrame->isLocalFrame()) {
        if (toLocalFrame(focusedFrame)->tree().isDescendantOf(frame()))
            return true;
    }
    return false;
}
