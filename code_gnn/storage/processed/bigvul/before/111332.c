void WebPage::setColorInput(const BlackBerry::Platform::String& value)
{
    if (d->m_page->defersLoading()) {
        d->m_deferredTasks.append(adoptPtr(new DeferredTaskSetColorInput(d, value)));
        return;
    }
    DeferredTaskSetColorInput::finishOrCancel(d);
    d->m_inputHandler->setInputValue(value);
}
