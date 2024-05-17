void WebPagePrivate::selectionChanged(Frame* frame)
{
    m_inputHandler->selectionChanged();

    m_page->focusController()->setFocusedFrame(frame);
}
