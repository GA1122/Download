bool WebPage::keyEvent(const Platform::KeyboardEvent& keyboardEvent)
{
    if (!d->m_mainFrame->view())
        return false;

    if (d->m_page->defersLoading())
        return false;

    ASSERT(d->m_page->focusController());

    bool handled = d->m_inputHandler->handleKeyboardInput(keyboardEvent);

    if (!handled && keyboardEvent.type() == Platform::KeyboardEvent::KeyDown && !d->m_inputHandler->isInputMode()) {
        IntPoint previousPos = d->scrollPosition();
        handleScrolling(keyboardEvent.character(), d);
        handled = previousPos != d->scrollPosition();
    }

    return handled;
}
